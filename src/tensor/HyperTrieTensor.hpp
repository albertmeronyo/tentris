#ifndef LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
#define LIBSPARSETENSOR_HYPERTRIETENSOR_HPP


#include "Tensor.hpp"
#include "../hypertrie/HyperTrie.hpp"
#include "../util/ExtendContainerPrint.hpp"
#include "Types.hpp"

#include <cstdint>
#include <vector>
#include <map>

using std::vector;
using sparsetensor::hypertrie::HyperTrie;

namespace sparsetensor::tensor {

    template<typename T>
    class HyperTrieTensor : public Tensor<T, HyperTrieTensor> {
    public:

        explicit HyperTrieTensor(const vector<uint64_t> &shape) : Tensor<T, HyperTrieTensor>{shape},
                                                                  trie(new HyperTrie<T>(uint8_t(shape.size()))) {}

        HyperTrieTensor(const vector<uint64_t> &shape, HyperTrie<T> *trie) : Tensor<T, HyperTrieTensor>{shape},
                                                                             trie(trie) {}

    private:
        HyperTrie<T> *trie;
    public:


        inline T get(vector<uint64_t> &key) {
            const optional<variant<HyperTrie<T> *, T>> &value_ = trie->get(key);
            if (value_) {
                return std::get<T>(*value_);
            } else {
                return {};
            }
        }

        inline void set(std::vector<uint64_t> &key, T &value) {
            if (value != T{}) {
                trie->set(key, value);
            } else { // new value is zero
                trie->del(key);
            }
            this->nnz = trie->leafcount;
            this->sum = trie->leafsum;
        }

        friend ostream &operator<<(ostream &out, HyperTrieTensor<T> &tensor) {
            out << "<Tensor: shape=" << tensor.shape << ", nnz=" << tensor.nnz << ", sum=" << tensor.sum << ">";
            return out;
        }
    };
}

#endif //LIBSPARSETENSOR_HYPERTRIETENSOR_HPP
