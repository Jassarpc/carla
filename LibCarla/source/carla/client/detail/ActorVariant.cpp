// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/client/detail/ActorVariant.h"

#include "carla/client/detail/ActorFactory.h"
#include "carla/client/ActorList.h"

namespace carla {
namespace client {
namespace detail {

  void ActorVariant::MakeActor(EpisodeProxy episode, SharedPtr<const client::ActorList> actor_list) const {
    auto const parent_id = GetParentId();
    SharedPtr<client::Actor> parent = nullptr;
    if ((actor_list != nullptr) && (parent_id != 0)) {
      // In case we have an actor list as context, we are able to actually
      // create the parent actor.
      parent = actor_list->Find(parent_id);
    }
    _value = detail::ActorFactory::MakeActor(
        episode,
        boost::get<rpc::Actor>(std::move(_value)),
        parent,
        GarbageCollectionPolicy::Disabled);
  }

} // namespace detail
} // namespace client
} // namespace carla