#ifndef Rcpp_macros_macros_h
#define Rcpp_macros_macros_h

#define RCPP_GET_NAMES(x)       Rf_getAttrib(x, R_NamesSymbol)

#ifndef BEGIN_RCPP
#define BEGIN_RCPP try{ 
#endif 

#ifndef VOID_END_RCPP
#define VOID_END_RCPP } catch( std::exception& __ex__ ){ Rcpp::forward_exception_to_r( __ex__ ) ; } catch(...){ ::Rf_error( "c++ exception (unknown reason)" ) ; }
#endif

#ifndef END_RCPP
#define END_RCPP VOID_END_RCPP return R_NilValue;
#endif

#ifndef END_RCPP_RETURN_ERROR
#define END_RCPP_RETURN_ERROR } catch( std::exception& __ex__ ){ return Rcpp::exception_to_try_error( __ex__ ) ; } catch(...){ return Rcpp::string_to_try_error( "c++ exception (unknown reason)" ) ; } return R_NilValue;
#endif

#include <Rcpp/macros/debug.h>
#include <Rcpp/macros/interface.h>
#include <Rcpp/macros/get_callable.h>

#endif
