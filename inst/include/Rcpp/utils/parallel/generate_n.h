#ifndef RCPP11_TOOLS_generate_PARALLEL_H
#define RCPP11_TOOLS_generate_PARALLEL_H

namespace Rcpp{
    namespace parallel{
        
        #if defined(RCPP11_EXPERIMENTAL_PARALLEL)
        template <typename OutputIterator, typename Size, typename Generator>
        inline void generate_n( int nthreads, OutputIterator begin, R_xlen_t n, Generator gen ){ 
            std::vector<std::thread> workers(nthreads-1) ;
            R_xlen_t chunk_size = n / nthreads ;
            R_xlen_t pos = 0;
            for( int i=0; i<nthreads-1; i++, pos += chunk_size){
                workers[i] = std::thread( std::generate_n<OutputIterator, R_xlen_t, Generator>, 
                    begin + pos, chunk_size, gen) ;   
            }
            std::generate_n( begin + pos, n-pos, gen ) ;
            for( int i=0; i<nthreads-1; i++) workers[i].join() ;
        }
        #else
        template <typename OutputIterator, typename Size, typename Generator>
        inline void generate_n( int, OutputIterator begin, R_xlen_t n, Generator gen ){ 
            std::generate_n( begin, n, gen ) ;
        }
        #endif
    
    }    
}

#endif
