/**
 * Scheme number symbol.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_number pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_NUMBER_H__
#define __SCHEME_NUMBER_H__

#include "scheme-element.h"

// Scheme number symbol.
typedef struct scheme_number scheme_number;

/**
 * Create new Scheme number symbol.
 *
 * Returned pointer must be freed with scheme_element_free().
 *
 * @param  value  Symbol's initial value.
 *
 * @return Newly created symbol, or NULL if out of memory.
 */
scheme_number *scheme_number_new(long value);

/**
 * Get number symbol's value.
 *
 * @param  symbol  A number symbol.
 */
long scheme_number_get_value(scheme_number *symbol);

/**
 * Get number symbol's type.
 *
 * @return Number symbol's type.
 */
scheme_element_type *scheme_number_get_type();

#endif
