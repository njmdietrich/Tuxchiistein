// SPDX-License-Identifier: GPL-3.0-or-later

#include <span>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "shader_recompiler/info_serialize.h"

TEST_CASE("Recipe shader infos retain upstream translation and binding flags",
          "[video_core][precompiled_info]") {
    Shader::Info original{};
    original.position_on_far_plane = true;
    original.demoted_conditional_barrier = true;
    original.uses_barrier_vote = true;
    original.binds_unmapped_pointer_word = true;
    original.requires_coherent_global_memory = true;
    original.frag_color_components[2] = 0x9;
    original.constant_buffer_used_sizes[1] = 256;
    original.handle_programs = {{1, 2, 3}, {4, 5}};
    Shader::StorageBufferDescriptor storage{};
    storage.cbuf_index = 1;
    storage.cbuf_offset = 0x80;
    storage.count = 1;
    storage.is_written = true;
    storage.indirect_offset = 16;
    storage.write_extent.bounded = true;
    storage.write_extent.guarded = true;
    storage.write_extent.const_end = 64;
    storage.write_extent.local_stride = {4, 8, 16};
    storage.write_extent.group_stride = {128, 256, 512};
    original.storage_buffers_descriptors.push_back(storage);

    std::vector<u8> bytes;
    Shader::SerializeInfo(original, bytes);
    std::span<const u8> input{bytes};
    Shader::Info restored{};
    REQUIRE(Shader::DeserializeInfo(input, restored));
    CHECK(input.empty());
    CHECK(restored == original);
}

TEST_CASE("Truncated recipe shader infos are rejected", "[video_core][precompiled_info]") {
    std::vector<u8> bytes;
    Shader::SerializeInfo(Shader::Info{}, bytes);
    for (size_t length = 0; length < bytes.size(); ++length) {
        std::span<const u8> input{bytes.data(), length};
        Shader::Info restored{};
        REQUIRE_FALSE(Shader::DeserializeInfo(input, restored));
    }
}
