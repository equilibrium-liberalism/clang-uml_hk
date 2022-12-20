/**
 * src/common/model/package.cc
 *
 * Copyright (c) 2021-2022 Bartek Kryza <bkryza@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "package.h"

#include <sstream>

namespace clanguml::common::model {
package::package(const common::model::namespace_ &using_namespace)
    : element{using_namespace}
{
}

std::string package::full_name(bool relative) const
{
    if (relative) {
        auto res = get_namespace().relative_to(using_namespace()) | name();
        return res.to_string();
    }

    return (get_namespace() | name()).to_string();
}

bool package::is_deprecated() const { return is_deprecated_; }

void package::set_deprecated(bool deprecated) { is_deprecated_ = deprecated; }

} // namespace clanguml::common::model