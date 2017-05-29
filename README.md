# Swizzle

An on-wire message description language and transpiler.

### Dependencies 

- c++14
- [UsingIntrusivePtrIn](https://github.com/paxos1977/UsingIntrusivePtrIn)
- [boost](http://boost.org). Uses intrusive_ptr in production code by default. Uses filesystem, bind, and asio in example code.
- [safe_numerics](https://github.com/robertramey/safe_numerics) will most likely be accepted to boost, but is currently in the incubator. Expecting headers to be in `boost/numeric/safe/`
- [ObjectGraph](https://github.com/paxos1977/ObjectGraph)

Used for unit testing on all platforms:

- [UnitTest++](https://github.com/unittest-cpp/unittest-cpp). Unit test framework.

### Contributors 

Austin Gilbert <ceretullis@gmail.com>

### License

4-Clause BSD license, see [LICENSE.md](LICENSE.md) for details. Other licensing available upon request. 
