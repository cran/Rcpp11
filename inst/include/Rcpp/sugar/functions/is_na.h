#ifndef Rcpp__sugar__is_na_h
#define Rcpp__sugar__is_na_h

namespace Rcpp{
    namespace sugar{
    
        template <int RTYPE, bool NA, typename VEC_TYPE>
        class IsNa : 
            public SugarVectorExpression< LGLSXP, false, IsNa<RTYPE,NA,VEC_TYPE> >, 
            public custom_sugar_vector_expression {
        public:
            typedef typename traits::storage_type<RTYPE>::type STORAGE ;
            typedef Rcpp::VectorBase<RTYPE,NA,VEC_TYPE> BASE ;
        
            IsNa( const BASE& obj_) : obj(obj_){}
        
            inline int operator[]( int i ) const {
                return ::Rcpp::traits::is_na<RTYPE>( obj[i] ) ;
            }
        
            inline int size() const { return obj.size() ; }
        
            template <typename Target>
            inline void apply( Target& target ) const {
                std::transform( sugar_begin(obj), sugar_end(obj), target.begin(), ::Rcpp::traits::is_na<RTYPE> );  
            }
            
        private:
            const BASE& obj ;
        
        } ;
              
        // specialization for the case where we already know
        // the result (FALSE) because it is embedded in the type
        // (the second template parameter of VectorBase)
        template <int RTYPE, typename VEC_TYPE>
        class IsNa<RTYPE,false,VEC_TYPE> : 
            public SugarVectorExpression< LGLSXP, false, IsNa<RTYPE,false,VEC_TYPE> >, 
            public custom_sugar_vector_expression {
        public:
            typedef typename traits::storage_type<RTYPE>::type STORAGE ;
            typedef Rcpp::VectorBase<RTYPE,false,VEC_TYPE> BASE ;
        
            IsNa( const BASE& obj_) : obj(obj_){}
        
            inline int operator[]( int i ) const {
                return FALSE ;
            }
        
            inline int size() const { return obj.size() ; }
        
            template <typename Target>
            inline void apply( Target& target ) const {
                std::fill( target.begin(), target.end(), FALSE);  
            }
            
        private:
            const BASE& obj ;
        
        } ;
        
    } // sugar
    
    template <int RTYPE, bool NA, typename T>
    inline sugar::IsNa<RTYPE,NA,T> is_na( const Rcpp::VectorBase<RTYPE,NA,T>& t){
        return sugar::IsNa<RTYPE,NA,T>( t ) ;
    }

} // Rcpp
#endif

