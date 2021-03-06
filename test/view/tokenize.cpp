#include <range/v3/core.hpp>
#include <range/v3/view/tokenize.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"

int main()
{
    using namespace ranges;

    std::string txt{"abc\ndef\tghi"};
    const std::regex rx{R"delim(([\w]+))delim"};
    auto&& rng = txt | view::tokenize(rx,1);
    const auto& crng = txt | view::tokenize(rx,1);

    ::check_equal(rng, {"abc","def","ghi"});
    ::check_equal(crng, {"abc","def","ghi"});

    ::has_type<const std::sub_match<std::string::iterator>&>(*ranges::begin(rng));
    ::has_type<const std::sub_match<std::string::iterator>&>(*ranges::begin(crng));

    ::models<concepts::BoundedRange>(rng);
    ::models<concepts::ForwardRange>(rng);
    ::models_not<concepts::BidirectionalRange>(rng);
    ::models_not<concepts::SizedRange>(rng);
    ::models_not<concepts::OutputRange>(rng);

    ::models<concepts::BoundedRange>(crng);
    ::models<concepts::ForwardRange>(crng);
    ::models_not<concepts::BidirectionalRange>(crng);
    ::models_not<concepts::SizedRange>(crng);
    ::models_not<concepts::OutputRange>(crng);

    // ::models<concepts::View>(rng);
    // ::models<concepts::View>(crng);

    return test_result();
}
