#ifndef Rcpp__vector_concat_h
#define Rcpp__vector_concat_h

namespace Rcpp{
    namespace traits{

        template <int RTYPE, typename T, bool prim>
        struct is_compatible_type {
            typedef typename std::is_same<typename T::stored_type, typename storage_type<RTYPE>::type >::type type ;
        } ;
        template <int RTYPE, typename T>
        struct is_compatible_type<RTYPE,T,true> {
            typedef typename std::is_same<T, typename storage_type<RTYPE>::type >::type type ;
        } ;

        template <int RTYPE, typename T>
        struct is_compatible {
            typedef typename is_compatible_type<RTYPE,T,traits::is_primitive<T>::value>::type type ;
        } ;

        template <int RTYPE, typename... Args>
        struct all_compatible {
            typedef typename and_< typename is_compatible<RTYPE,typename traits::remove_const_and_reference<Args>::type >::type ... >::type type;
        } ;
        
        // not a primitive
        template <typename T, bool prim>
        struct get_compatible_r_vector_type__dispatch {
            enum { 
                rtype = r_sexptype_traits<typename T::stored_type>::rtype 
            } ; 
        } ;
        
        // a primitive
        template <typename T>
        struct get_compatible_r_vector_type__dispatch<T,true> {
            enum { 
                rtype = r_sexptype_traits<T>::rtype 
            } ; 
        } ;
        
        template <typename T>
        struct get_compatible_r_vector_type {
            enum{
               rtype = get_compatible_r_vector_type__dispatch<T, is_primitive<T>::value >::rtype  
            } ;
        } ;
        
        
    }

    template <typename T>
    inline int get_size_one( const T&, std::true_type, std::true_type ){
        return 1 ;
    }

    template <typename T>
    inline int get_size_one( const T&, std::true_type, std::false_type ){
        return 1 ;
    }

    template <typename T>
    inline int get_size_one( const T& obj, std::false_type, std::true_type ){
        return Rf_length(obj);
    }

    template <typename T>
    inline int get_size_one( const T& obj, std::false_type, std::false_type ){
        return obj.size();
    }

    template <typename First>
    int get_size( const First& first){
        return get_size_one<First>(first, typename traits::is_primitive<First>::type(), typename std::is_same<First, SEXP>::type() ) ;
    }

    template <typename First, typename... Rest>
    int get_size( const First& first, Rest... rest ){
        return get_size_one<First>(first, typename traits::is_primitive<First>::type(), typename std::is_same<First, SEXP>::type() ) + get_size(rest...) ;
    }

    template <int RTYPE, typename... Args>
    Vector<RTYPE> concatenate(Args... args) {
        static_assert( traits::all_compatible<RTYPE,Args...>::type::value, "incompatible parameters" ) ;

        int n = get_size(args...) ;
        Vector<RTYPE> out(n) ;
        int i = 0 ;
        if (sizeof...(args)) {
            do_concatenate(out, i, args...);
        }

        return out ;
    }
    
    template <typename T, typename Current, typename... Rest>
    void do_concatenate_one( T& x, int& idx, const Current& curr, std::false_type, std::false_type ){
        int n = curr.size();
        for (int i=0; i < n; ++i, idx++) {
            x[idx] = curr[i];
        }
    }

    template <typename T, typename Current, typename... Rest>
    void do_concatenate_one( T& x, int& idx, const Current& curr, std::false_type, std::true_type ){
        int n = Rf_length(curr);
        T Tcurr = as<T>(curr);
        for (int i=0; i < n; ++i, idx++) {
            x[idx] = Tcurr[i];
        }
    }

    template <typename T, typename Current, typename... Rest>
    void do_concatenate_one( T& x, int& idx, const Current& curr, std::true_type, std::false_type ){
        x[idx++]=curr ;
    }

    template <typename T, typename Current, typename... Rest>
    void do_concatenate_one( T& x, int& idx, const Current& curr, std::true_type, std::true_type ){
        x[idx++]=curr ;
    }

    template <typename T, typename Current, typename... Rest>
    void do_concatenate(T& x, int& idx, const Current& curr, Rest... rest) {
        do_concatenate_one(x, idx, curr, typename traits::is_primitive<Current>::type(), typename std::is_same<Current, SEXP>::type() );
        do_concatenate(x, idx, rest...);
    }

    template <typename T, typename Current>
    void do_concatenate(T& x, int& idx, const Current& curr){
        do_concatenate_one(x, idx, curr, typename traits::is_primitive<Current>::type(), typename std::is_same<Current, SEXP>::type() );
    }

    template <typename First, typename... Args>
    typename std::enable_if< 
        traits::all_compatible< traits::get_compatible_r_vector_type<First>::rtype, Args...>::type::value,
        Vector< traits::get_compatible_r_vector_type<First>::rtype >
    >::type
    fuse( const First& first, Args... args ){
        return concatenate< traits::get_compatible_r_vector_type<First>::rtype, First, Args...>( first, args... ) ;  
    }
    
}

#endif
