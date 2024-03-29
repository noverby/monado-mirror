// Copyright 2019, Collabora, Ltd.
// SPDX-License-Identifier: BSL-1.0
/*!
 * @file
 * @brief  Hashmap for integer values header.
 * @author Jakob Bornecrantz <jakob@collabora.com>
 * @author Korcan Hussein <korcan.hussein@collabora.com>
 * @ingroup aux_util
 */

#pragma once

#include "xrt/xrt_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * @struct u_hashmap_int
 * @ingroup aux_util
 *
 * A simple uint64_t key to a void pointer hashmap.
 */
struct u_hashmap_int;

typedef void (*u_hashmap_int_callback)(void *item, void *priv);
typedef void (*u_hashmap_int_foreach_callback)(uint64_t key, const void *value, void *priv_ctx);

int
u_hashmap_int_create(struct u_hashmap_int **out_hashmap);

int
u_hashmap_int_destroy(struct u_hashmap_int **hmi);

int
u_hashmap_int_find(struct u_hashmap_int *hmi, uint64_t key, void **out_item);

int
u_hashmap_int_insert(struct u_hashmap_int *hmi, uint64_t key, void *value);

int
u_hashmap_int_erase(struct u_hashmap_int *hmi, uint64_t key);

/*!
 * Is the hash map empty?
 */
bool
u_hashmap_int_empty(const struct u_hashmap_int *hmi);

/*!
 * iterators through each [key,item] pairs of hash map
 * @param  hmi       hash map to iterate
 * @param  cb        callback invoked for each [key,item] pair + a user context.
 * @param  priv_ctx  user provided context, passed into `cb`
 * @ingroup aux_util
 */
void
u_hashmap_int_for_each(const struct u_hashmap_int *hmi, u_hashmap_int_foreach_callback cb, void *priv_ctx);

/*!
 * First clear the hashmap and then call the given callback with each item that
 * was in the hashmap.
 *
 * @ingroup aux_util
 */
void
u_hashmap_int_clear_and_call_for_each(struct u_hashmap_int *hmi, u_hashmap_int_callback cb, void *priv);


#ifdef __cplusplus
}
#endif
