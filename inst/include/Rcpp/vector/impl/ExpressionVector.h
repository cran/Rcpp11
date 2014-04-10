#ifndef Rcpp__vector__impl_ExpressionVector_h
#define Rcpp__vector__impl_ExpressionVector_h

namespace Rcpp{
        
    #undef VEC
    #define VEC Vector<EXPRSXP,StoragePolicy>
    template <
        template <class> class StoragePolicy
    >
    class Vector<EXPRSXP,StoragePolicy> :
        public VectorBase< EXPRSXP, true, VEC>,
        public StoragePolicy<VEC>,
        public SlotProxyPolicy<VEC>,
        public AttributeProxyPolicy<VEC>,
        public NamesProxyPolicy<VEC>,
        public AttributesProxyPolicy<VEC>, 
        public RObjectMethods<VEC>, 
        public VectorOffset<VEC>
    {
    public:
        
        using value_type      = SEXP ;
        using stored_type     = SEXP ;
        using init_type       = SEXP ;
        using Proxy           = internal::generic_proxy<EXPRSXP> ;
        using const_Proxy     = internal::const_generic_proxy<EXPRSXP> ;
        using iterator        = internal::Proxy_Iterator<Proxy> ; 
        using const_iterator  = internal::const_Proxy_Iterator<const_Proxy> ;
        using NameProxy       = internal::generic_name_proxy<EXPRSXP> ;
        using const_NameProxy = internal::generic_const_name_proxy<EXPRSXP> ;
        
        using VectorOffset<Vector>::size ;
        
        RCPP_GENERATE_CTOR_ASSIGN(Vector)
    
        Vector( SEXP x ) {
            Storage::set__( r_cast<EXPRSXP>( x ) ) ;
        }
        
        Vector( int n ) : Vector(Rf_allocVector(EXPRSXP, n) ) {}
        Vector() : Vector(0) {}
    
        Vector( const char* st){
            ParseStatus status;
            Shield<SEXP> expr = Rf_mkString( st );
            Shield<SEXP> res  = R_ParseVector(expr, -1, &status, R_NilValue);
            if( status != PARSE_OK ){
                throw parse_error() ;
            }
            Storage::set__(res) ;
        }
        Vector( const std::string& st) : Vector( st.c_str()){} ;
        
        template <bool NA, typename Expr>
        Vector( const SugarVectorExpression<EXPRSXP,NA,Expr>& other ) : Vector(other.size()) {
            other.apply(*this) ;
        }
    
        template <bool NA, typename Expr>
        Vector& operator=( const SugarVectorExpression<EXPRSXP, NA, Expr>& expr ){
            int n = expr.size() ;
            if( n != size() ){
                reset(n) ;    
            }
            expr.apply(*this) ;
            return *this ;
        }
        
    private:
        
        inline void reset(int n){
            Storage::set__(Rf_allocVector(EXPRSXP, n) ) ;        
        }
        
        inline stored_type* data(){
            return reinterpret_cast<stored_type*>( DATAPTR(Storage::get__()) );    
        }
        inline const stored_type* data() const{
            return reinterpret_cast<const stored_type*>( DATAPTR(Storage::get__()) );    
        }
        
    public:
        
        inline iterator begin() { 
            return iterator( Proxy(*this, 0) ); 
        }
        inline iterator end() { 
            return iterator( Proxy(*this, size() ) ); 
        }
        
        inline const_iterator begin() const{ 
            return const_iterator( const_Proxy(*this, 0) );
        }
        inline const_iterator end() const{ 
            return const_iterator( const_Proxy(*this, size() ) );
        }
        
        inline Proxy operator[](int i){ 
            return Proxy(*this, i ) ;
        }
        inline const_Proxy operator[](int i) const { 
            return const_Proxy(*this, size() ) ; 
        }
        
        template <typename... Args> static Vector create(Args... args) {
            return typename create_type<EXPRSXP, Args...>::type( args... ) ;    
        }
    
        inline NameProxy operator[]( const std::string& name ){
            return NameProxy( *this, name ) ;
        }
        inline const_NameProxy operator[]( const std::string& name ) const {
            return const_NameProxy( *this, name ) ;
        }
    } ;
    #undef VEC  
}
#endif
