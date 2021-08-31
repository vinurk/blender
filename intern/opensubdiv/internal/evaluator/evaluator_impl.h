// Copyright 2018 Blender Foundation. All rights reserved.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// Author: Sergey Sharybin

#ifndef OPENSUBDIV_EVALUATOR_IMPL_H_
#define OPENSUBDIV_EVALUATOR_IMPL_H_

#ifdef _MSC_VER
#  include <iso646.h>
#endif

#include <opensubdiv/far/patchMap.h>
#include <opensubdiv/far/patchTable.h>

#include "internal/base/memory.h"

struct OpenSubdiv_BufferInterface;
struct OpenSubdiv_PatchCoord;
struct OpenSubdiv_TopologyRefiner;

class PatchMap;

namespace blender {
namespace opensubdiv {

// Anonymous forward declaration of actual evaluator implementation.
class CpuEvalOutput;
class GpuEvalOutput;

// Wrapper around implementaiton, which defines API which we are capable to
// provide over the implementation.
//
// TODO(sergey):  It is almost the same as C-API object, so ideally need to
// merge them somehow, but how to do this and keep files with all the templates
// and such separate?
class CpuEvalOutputAPI {
 public:
  // NOTE: API object becomes an owner of evaluator. Patch we are referencing.
  CpuEvalOutputAPI(CpuEvalOutput *implementation, PatchMap *patch_map);
  ~CpuEvalOutputAPI();

  // Set coarse positions from a continuous array of coordinates.
  void setCoarsePositions(const float *positions,
                          const int start_vertex_index,
                          const int num_vertices);
  // Set varying data from a continuous array of data.
  void setVaryingData(const float *varying_data,
                      const int start_vertex_index,
                      const int num_vertices);
  // Set face varying data from a continuous array of data.
  //
  // TODO(sergey): Find a better name for vertex here. It is not the vertex of
  // geometry, but a vertex of UV map.
  void setFaceVaryingData(const int face_varying_channel,
                          const float *varying_data,
                          const int start_vertex_index,
                          const int num_vertices);

  // Set coarse vertex position from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  void setCoarsePositionsFromBuffer(const void *buffer,
                                    const int start_offset,
                                    const int stride,
                                    const int start_vertex_index,
                                    const int num_vertices);
  // Set varying data from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  void setVaryingDataFromBuffer(const void *buffer,
                                const int start_offset,
                                const int stride,
                                const int start_vertex_index,
                                const int num_vertices);
  // Set face varying data from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  //
  // TODO(sergey): Find a better name for vertex here. It is not the vertex of
  // geometry, but a vertex of UV map.
  void setFaceVaryingDataFromBuffer(const int face_varying_channel,
                                    const void *buffer,
                                    const int start_offset,
                                    const int stride,
                                    const int start_vertex_index,
                                    const int num_vertices);

  // Refine after coarse positions update.
  void refine();

  // Evaluate given ptex face at given bilinear coordinate.
  // If derivatives are NULL, they will not be evaluated.
  void evaluateLimit(const int ptex_face_index,
                     float face_u,
                     float face_v,
                     float P[3],
                     float dPdu[3],
                     float dPdv[3]);

  // Evaluate varying data at a given bilinear coordinate of given ptex face.
  void evaluateVarying(const int ptes_face_index, float face_u, float face_v, float varying[3]);

  // Evaluate facee-varying data at a given bilinear coordinate of given
  // ptex face.
  void evaluateFaceVarying(const int face_varying_channel,
                           const int ptes_face_index,
                           float face_u,
                           float face_v,
                           float face_varying[2]);

  // Batched evaluation of multiple input coordinates.

  // Evaluate given ptex face at given bilinear coordinate.
  // If derivatives are NULL, they will not be evaluated.
  //
  // NOTE: Output arrays must point to a memory of size float[3]*num_patch_coords.
  void evaluatePatchesLimit(const OpenSubdiv_PatchCoord *patch_coords,
                            const int num_patch_coords,
                            float *P,
                            float *dPdu,
                            float *dPdv);

 protected:
  CpuEvalOutput *implementation_;
  PatchMap *patch_map_;
};

class GpuEvalOutputAPI {
 public:
  // NOTE: API object becomes an owner of evaluator. Patch we are referencing.
  GpuEvalOutputAPI(GpuEvalOutput *implementation, PatchMap *patch_map);
  ~GpuEvalOutputAPI();

  // Set coarse positions from a continuous array of coordinates.
  void setCoarsePositions(const float *positions,
                          const int start_vertex_index,
                          const int num_vertices);
  // Set varying data from a continuous array of data.
  void setVaryingData(const float *varying_data,
                      const int start_vertex_index,
                      const int num_vertices);
  // Set face varying data from a continuous array of data.
  //
  // TODO(sergey): Find a better name for vertex here. It is not the vertex of
  // geometry, but a vertex of UV map.
  void setFaceVaryingData(const int face_varying_channel,
                          const float *varying_data,
                          const int start_vertex_index,
                          const int num_vertices);

  // Set coarse vertex position from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  void setCoarsePositionsFromBuffer(const void *buffer,
                                    const int start_offset,
                                    const int stride,
                                    const int start_vertex_index,
                                    const int num_vertices);
  // Set varying data from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  void setVaryingDataFromBuffer(const void *buffer,
                                const int start_offset,
                                const int stride,
                                const int start_vertex_index,
                                const int num_vertices);
  // Set face varying data from a continuous memory buffer where
  // first coordinate starts at offset of `start_offset` and there is `stride`
  // bytes between adjacent vertex coordinates.
  //
  // TODO(sergey): Find a better name for vertex here. It is not the vertex of
  // geometry, but a vertex of UV map.
  void setFaceVaryingDataFromBuffer(const int face_varying_channel,
                                    const void *buffer,
                                    const int start_offset,
                                    const int stride,
                                    const int start_vertex_index,
                                    const int num_vertices);

  // Refine after coarse positions update.
  void refine();

  // Evaluate given ptex face at given bilinear coordinate.
  // If derivatives are NULL, they will not be evaluated.
  void evaluateLimit(const int ptex_face_index,
                     float face_u,
                     float face_v,
                     float P[3],
                     float dPdu[3],
                     float dPdv[3]);

  // Evaluate varying data at a given bilinear coordinate of given ptex face.
  void evaluateVarying(const int ptes_face_index, float face_u, float face_v, float varying[3]);

  // Evaluate facee-varying data at a given bilinear coordinate of given
  // ptex face.
  void evaluateFaceVarying(const int face_varying_channel,
                           const int ptes_face_index,
                           float face_u,
                           float face_v,
                           float face_varying[2]);

  void evaluateFaceVarying(const int face_varying_channel,
                           OpenSubdiv_BufferInterface *patch_coords_buffer,
                           OpenSubdiv_BufferInterface *face_varying);

  // Batched evaluation of multiple input coordinates.

  // Evaluate given ptex face at given bilinear coordinate.
  // If derivatives are NULL, they will not be evaluated.
  //
  // NOTE: Output arrays must point to a memory of size float[3]*num_patch_coords.
  void evaluatePatchesLimit(const OpenSubdiv_PatchCoord *patch_coords,
                            const int num_patch_coords,
                            float *P,
                            float *dPdu,
                            float *dPdv);

  void evaluatePatchesLimit(OpenSubdiv_BufferInterface *patch_coords,
                            OpenSubdiv_BufferInterface *P,
                            OpenSubdiv_BufferInterface *dPdu,
                            OpenSubdiv_BufferInterface *dPdv);

  void getPatchMap(OpenSubdiv_BufferInterface *patch_map_handles,
                   OpenSubdiv_BufferInterface *patch_map_quadtree,
                   int *min_patch_face,
                   int *max_patch_face,
                   int *max_depth,
                   int *patches_are_triangular);

  void buildPatchArraysBuffer(OpenSubdiv_BufferInterface *patch_arrays_buffer);

  void buildPatchIndexBuffer(OpenSubdiv_BufferInterface *patch_index_buffer);

  void buildPatchParamBuffer(OpenSubdiv_BufferInterface *patch_param_buffer);

  void buildSrcBuffer(OpenSubdiv_BufferInterface *src_buffer);

  void buildFVarPatchArraysBuffer(const int face_varying_channel,
                                  OpenSubdiv_BufferInterface *patch_arrays_buffer);

  void buildFVarPatchIndexBuffer(const int face_varying_channel,
                                 OpenSubdiv_BufferInterface *patch_index_buffer);

  void buildFVarPatchParamBuffer(const int face_varying_channel,
                                 OpenSubdiv_BufferInterface *patch_param_buffer);

  void buildFVarSrcBuffer(const int face_varying_channel, OpenSubdiv_BufferInterface *src_buffer);

 protected:
  GpuEvalOutput *implementation_;
  PatchMap *patch_map_;
};

}  // namespace opensubdiv
}  // namespace blender

struct OpenSubdiv_EvaluatorImpl {
 public:
  OpenSubdiv_EvaluatorImpl();
  ~OpenSubdiv_EvaluatorImpl();

  blender::opensubdiv::GpuEvalOutputAPI *eval_output_gpu;
  blender::opensubdiv::CpuEvalOutputAPI *eval_output;
  const PatchMap *patch_map;
  const OpenSubdiv::Far::PatchTable *patch_table;

  MEM_CXX_CLASS_ALLOC_FUNCS("OpenSubdiv_EvaluatorImpl");
};

struct OpenSubdiv_EvaluatorCacheImpl {
 public:
  OpenSubdiv_EvaluatorCacheImpl();
  ~OpenSubdiv_EvaluatorCacheImpl();

  // TODO(kevindietrich)
  // typedef typename blender::opensubdiv::GpuEvalOutputAPI::EvaluatorCache EvaluatorCache;

  void *eval_cache;
  MEM_CXX_CLASS_ALLOC_FUNCS("OpenSubdiv_EvaluatorCacheImpl");
};

OpenSubdiv_EvaluatorImpl *openSubdiv_createEvaluatorInternal(
    struct OpenSubdiv_TopologyRefiner *topology_refiner,
    int evaluator_type,
    OpenSubdiv_EvaluatorCacheImpl *evaluator_cache_descr);

void openSubdiv_deleteEvaluatorInternal(OpenSubdiv_EvaluatorImpl *evaluator);

OpenSubdiv_EvaluatorCacheImpl *openSubdiv_createEvaluatorCacheInternal(int evaluator_type);

void openSubdiv_deleteEvaluatorCacheInternal(OpenSubdiv_EvaluatorCacheImpl *evaluator_cache);

#endif  // OPENSUBDIV_EVALUATOR_IMPL_H_