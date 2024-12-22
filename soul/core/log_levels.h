#ifndef LOGLEVELS_H
#define LOGLEVELS_H

#define LOG_LEVELS \
    X(LOG_DEBUGVERBOSE, "\033[32m", "DEBUGVERB")       /* Green */ \
    X(LOG_DEBUG, "\033[32m", "DEBUG")       /* Green */ \
    X(LOG_INFO, "\033[34m", "INFO")        /* Blue */ \
    X(LOG_WARNING, "\033[38;5;214m", "WARNING")  /* Orange (approximate) */ \
    X(LOG_ERROR, "\033[31m", "ERROR")       /* Red */ \

#endif // LOGLEVELS_H

/* X Macro !
The X Macro technique is particularly useful in scenarios 
where you need to define a list of related data or configurations 
in a concise and maintainable way. 
It allows you to avoid repetitive code and easily update or modify 
the list by changing the template macro. 
Additionally, it helps in ensuring consistency and reducing the 
likelihood of errors when managing large sets of data or configurations.
*/
