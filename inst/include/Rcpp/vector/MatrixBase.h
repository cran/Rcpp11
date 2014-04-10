#ifndef Rcpp__vector__MatrixBase_h
#define Rcpp__vector__MatrixBase_h

namespace Rcpp{

    template <int RTYPE, bool NA, typename Matrix>
    class MatrixBase : public CRTP<Matrix> {
    public:
        using CRTP<Matrix>::get_ref ;
        using stored_type  = typename traits::storage_type<RTYPE>::type ;
        
        inline stored_type operator()(int i, int j) const { 
            return get_ref()(i,j) ; 
        }
        inline int nrow() const { 
            return get_ref().nrow() ; 
        }
        inline int ncol() const { 
            return get_ref().ncol() ; 
        }
        inline int size() const { 
            return get_ref().size() ; 
        }
        
    } ;
    
}

#endif
