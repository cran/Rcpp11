#ifndef RCPP11_TOOLS_IOTA_PARALLEL_H
#define RCPP11_TOOLS_IOTA_PARALLEL_H

namespace Rcpp{
    namespace parallel{
             
        #if defined(RCPP11_EXPERIMENTAL_PARALLEL)
        template <typename OutputIterator, typename T>
        inline void iota( int nthreads, OutputIterator begin, OutputIterator end, T start ){ 
            std::vector<std::thread> workers(nthreads-1) ;
            R_xlen_t chunk_size = std::distance(begin, end) / nthreads ;
            R_xlen_t pos = 0;
            for( int i=0; i<nthreads-1; i++, pos += chunk_size){
                workers[i] = std::thread( std::iota<OutputIterator, T>, 
                    begin + pos, begin + pos + chunk_size, 
                    start + pos) ;   
            }
            std::iota( begin + pos, end, start + pos ) ;
            for( int i=0; i<nthreads-1; i++) workers[i].join() ;
        }
        #else
        template <typename OutputIterator, typename T>
        inline void iota( int, OutputIterator begin, OutputIterator end, T start ){ 
            std::iota( begin, end, start ) ;
        }
        #endif
    
    }    
}

#endif
