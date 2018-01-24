#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/nodes/Root.hpp>

#include <tuple>

namespace {
    
    using namespace swizzle;
    
    struct AncestorInfoFixture
    {
        ast::AncestorInfo info;
    };
    
    struct WhenAstIsEmpty : public AncestorInfoFixture
    {
    };
    
    TEST_FIXTURE(WhenAstIsEmpty, verifyDepth)
    {
        CHECK_EQUAL(0U, info.depth());
    }
    
    struct WhenAstHasOneNode : public AncestorInfoFixture
    {
        WhenAstHasOneNode()
        {
            info.push(node);
        }
        
        ast::nodes::Root node;
    };
    
    TEST_FIXTURE(WhenAstHasOneNode, verifyDepth)
    {
        CHECK_EQUAL(1U, info.depth());
    }
    
    struct WhenAstHasDepthTwo : public AncestorInfoFixture
    {
        WhenAstHasDepthTwo()
        {
            info.push(root);
            info.push(child);
        }
        
        ast::nodes::Root root;
        ast::nodes::Root child;
        ast::nodes::Root child2;
    };
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyDepthOf)
    {
        std::size_t depth = 100;
        bool found = false;
        
        std::tie(depth, found) = info.depth_of(root);
        CHECK_EQUAL(0U, depth);
        CHECK(found);
        
        std::tie(depth, found) = info.depth_of(child);
        CHECK_EQUAL(1U, depth);
        CHECK(found);
        
        std::tie(depth, found) = info.depth_of(child2);
        CHECK(!found);
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyFind)
    {
        const auto iter = info.find(child);
        
        CHECK(iter != info.cend());
        CHECK_EQUAL(&child, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyFindWhenNodeNotPresent)
    {
        const auto iter = info.find(child2);
        REQUIRE CHECK(iter == info.cend());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyParentOf)
    {
        const auto iter = info.parent_of(child);
        
        CHECK(iter != info.cend());
        CHECK_EQUAL(&root, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyParentOfRootIsRoot)
    {
        const auto iter = info.parent_of(root);
        
        REQUIRE CHECK(iter != info.cend());
        CHECK_EQUAL(&root, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyParentOfWhenNodeNotFound)
    {
        const auto iter = info.parent_of(child2);
        REQUIRE CHECK(iter == info.cend());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyPop)
    {
        CHECK_EQUAL(2U, info.depth());
        
        info.pop();
        CHECK_EQUAL(1U, info.depth());
    }
    
    TEST_FIXTURE(WhenAstIsEmpty, verifyPop)
    {
        CHECK_EQUAL(0U, info.depth());
        
        info.pop();
        CHECK_EQUAL(0U, info.depth());
    }
    
    struct WhenAstHasDepthThree : public AncestorInfoFixture
    {
        WhenAstHasDepthThree()
        {
            info.push(root);
            info.push(child);
            info.push(child2);
        }
        
        ast::nodes::Root root;
        ast::nodes::Root child;
        ast::nodes::Root child2;
    };
    
    TEST_FIXTURE(WhenAstHasDepthThree, verifyGrandparentOf)
    {
        const auto iter = info.grandparent_of(child2);
        
        REQUIRE CHECK(iter != info.cend());
        CHECK_EQUAL(&root, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyGrandparentOf)
    {
        const auto iter = info.grandparent_of(child);
        
        REQUIRE CHECK(iter != info.cend());
        CHECK_EQUAL(&root, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyGrandparentOfRootIsRoot)
    {
        const auto iter = info.grandparent_of(root);
        
        REQUIRE CHECK(iter != info.cend());
        CHECK_EQUAL(&root, &iter->get());
    }
    
    TEST_FIXTURE(WhenAstHasDepthTwo, verifyGrandparentOfReturnsEndOnNotFound)
    {
        const auto iter = info.grandparent_of(child2);
        REQUIRE CHECK(iter == info.cend());
    }
}
