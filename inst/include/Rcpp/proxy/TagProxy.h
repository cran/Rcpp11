#ifndef Rcpp_TagProxy_h
#define Rcpp_TagProxy_h

namespace Rcpp{

    template <typename XPtrClass>
    class TagProxyPolicy {
    public:
        
      class TagProxy : public GenericProxy<TagProxy>{
      public:
          TagProxy( XPtrClass& xp_ ): xp(xp_){}
          
          template <typename U>
          TagProxy& operator=( const U& rhs) {
              return set( wrap( rhs ) ) ;
          }
          
          template <typename U>
          operator U() const {
              return as<U>(get());  
          }
          
          operator SEXP() const { 
              return get(); 
          }
          
          
      private:
          XPtrClass& xp ;
          
          inline SEXP get() const {
              return R_ExternalPtrTag(xp) ;
          }
          
          inline TagProxy& set( SEXP x){
              R_SetExternalPtrTag( xp, x ) ;
              return *this ;
          }
      
      } ;
      
      class const_TagProxy : public GenericProxy<const_TagProxy>{
      public:
          const_TagProxy( XPtrClass& xp_ ): xp(xp_){}
          
          template <typename U>
          operator U() const {
              return as<U>(get());  
          }
          
          operator SEXP() const { 
              return get(); 
          }
          
          
      private:
          XPtrClass& xp ;
          
          inline SEXP get() const {
              return R_ExternalPtrTag(xp) ;
          }
          
      } ;
      
      
      TagProxy tag(){
          return TagProxy( static_cast<XPtrClass&>(*this) ) ;
      }
      
      const_TagProxy tag() const {
          return TagProxy( static_cast<const XPtrClass&>(*this) ) ;
      }
        
    
    } ;
}

#endif
