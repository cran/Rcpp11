#ifndef Rcpp__vector__simple_name_proxy_h
#define Rcpp__vector__simple_name_proxy_h

namespace Rcpp{
namespace internal{

    template <int RTYPE> class simple_name_proxy : public name_proxy<RTYPE,Vector<RTYPE>> {
    public:
        using name_proxy<RTYPE,Vector<RTYPE>>::set ;
        
        typedef ::Rcpp::Vector<RTYPE> VECTOR ;
        typedef typename ::Rcpp::traits::storage_type<RTYPE>::type CTYPE ;
    
        simple_name_proxy( VECTOR& v, std::string  name_) :
            parent(v), name(std::move(name_)){}
        simple_name_proxy( const simple_name_proxy& other ) : 
            parent(other.parent), name(other.name){}
        ~simple_name_proxy() {}
    
        simple_name_proxy& operator=( CTYPE rhs ){
            set( rhs ) ;
            return *this ;
        }
        simple_name_proxy& operator=( const simple_name_proxy& other){
            set( other.get() ) ;
            return *this ;
        }
        simple_name_proxy& operator=( const Na_Proxy& /* other */ ){
            set( Rcpp::traits::get_na<RTYPE>() );
            return *this ;
        }
    
        template <typename T>
        simple_name_proxy& operator=( const T& rhs ){
            set( caster<T,CTYPE>(rhs) ) ;
            return *this ;
        }
    
        operator CTYPE() const {
             return get() ;
        }
    
        operator SEXP() const {
            return ::Rcpp::wrap(get()) ;
        }
    
    private:
        VECTOR& parent ;
        std::string name;
    
        void set( CTYPE rhs ){
            set( parent, name, rhs) ;
        }
    
        CTYPE get() const {
          int i = parent.offset(name) ;
          return parent[i] ;
        }
    
    } ;

}
}
#endif
