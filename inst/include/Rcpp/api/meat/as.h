#ifndef Rcpp_api_meat_as_h
#define Rcpp_api_meat_as_h

namespace Rcpp{
namespace internal{

    template <> inline Rcpp::String as<Rcpp::String>(SEXP x, ::Rcpp::traits::r_type_string_tag ) {
        if( ! Rf_isString(x) ){
            throw ::Rcpp::not_compatible( "expecting a string" ) ;
        }
        if (Rf_length(x) != 1) {
            throw ::Rcpp::not_compatible( "expecting a single value");
        }
        return STRING_ELT( x, 0 ) ;
    }
    
}
}
#endif
