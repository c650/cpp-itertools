#pragma once

#include <functional>
#include <iterator>

namespace itertools {

  const auto iter_empty = std::runtime_error("iterator has no next element");

  template <typename T, typename ForwardIter>
  class filter_iter;

  template <typename T, typename U, typename ForwardIter>
  class map_iter;

  template <typename T, typename ForwardIter>
  class base_iter;

  template <typename T, typename ForwardIter>
  class i_base_iter {
  public:
    virtual T peek(void) = 0;
    virtual T next(void) = 0;
    virtual bool has_next(void) = 0;

    template <typename U>
    map_iter<T, U, ForwardIter> map(const std::function<U(const T&)>& closure) {
      return map_iter<T, U, ForwardIter>(*this, closure);
    }

    filter_iter<T, ForwardIter> filter(
        const std::function<bool(const T&)>& pred) {
      return filter_iter<T, ForwardIter>(*this, pred);
    }

    template <typename U>
    U reduce(const U& start, const std::function<U(U&, const T&)>& closure) {
      U ret{start};

      while (this->has_next()) {
        ret = closure(ret, this->next());
      }

      return ret;
    }

    template <typename Container>
    Container collect(void) {
      Container ret{};
      std::back_insert_iterator<Container> itr{std::back_inserter(ret)};
      while (this->has_next()) {
        itr = this->next();
      }
      return ret;
    }
  };

  template <typename T, typename ForwardIter>
  class base_iter : public i_base_iter<T, ForwardIter> {
  protected:
    ForwardIter cur;
    ForwardIter end;

  public:
    base_iter(ForwardIter start, ForwardIter end) : cur(start), end(end) {
    }

    base_iter(const base_iter<T, ForwardIter>& other)
        : base_iter(other.cur, other.end) {
    }

    T peek(void) override {
      if (!this->has_next()) {
        throw iter_empty;
      }

      return *cur;
    }

    T next(void) override {
      if (!this->has_next()) {
        throw iter_empty;
      }

      return *cur++;
    };

    bool has_next(void) override {
      return cur != end;
    };
  };

  template <typename T, typename U, typename ForwardIter>
  class map_iter : public i_base_iter<T, ForwardIter> {
    i_base_iter<T, ForwardIter>& _prev;
    std::function<U(const T&)> _closure;

  public:
    map_iter(i_base_iter<T, ForwardIter>& _prev,
             const std::function<U(const T&)>& _closure)
        : _prev(_prev), _closure(_closure) {
    }

    U peek(void) override {
      return this->_closure(this->_prev.peek());
    }

    U next(void) override {
      return this->_closure(this->_prev.next());
    }

    bool has_next(void) {
      return this->_prev.has_next();
    }
  };

  template <typename T, typename ForwardIter>
  class filter_iter : public i_base_iter<T, ForwardIter> {
    i_base_iter<T, ForwardIter>& _prev;
    std::function<bool(const T&)> _pred;

  public:
    filter_iter(i_base_iter<T, ForwardIter>& _prev,
                const std::function<bool(const T&)>& _pred)
        : _prev(_prev), _pred(_pred) {
    }

    T peek(void) override {
      if (!this->has_next()) {
        throw iter_empty;
      }

      return this->_prev.peek();
    }

    T next(void) override {
      if (!this->has_next()) {
        throw iter_empty;
      }

      return this->_prev.next();
    }

    bool has_next(void) override {
      while (this->_prev.has_next()) {
        if (this->_pred(this->_prev.peek())) {
          return true;
        }

        // throw away bad one
        this->_prev.next();
      }

      return false;
    }
  };

}; // namespace itertools
