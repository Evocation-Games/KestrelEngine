// Copyright (c) 2023 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <libTesting/testing.hpp>
#include <libKDL/lexer/lexeme_stream.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>

using namespace kdl::lexer;

// MARK: - Helpers

static auto lexeme_sequence_stub() -> std::vector<lexeme>
{
    std::vector<lexeme> sequence;
    sequence.emplace_back("type", lexeme::directive);
    sequence.emplace_back("FooBar", lexeme::identifier);
    sequence.emplace_back(":", lexeme::colon);
    sequence.emplace_back("fubr", lexeme::string);
    return sequence;
}


// MARK: - Tests

TEST(lexer_lexemeStream_constructedCorrectly)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::equal(stream.size(), 4);
}

TEST(lexer_lexemeStream_returnsExpectedLexemeAtIndex)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.at(2).is(lexeme::colon, ":"));
}

TEST(lexer_lexemeStream_reportsExpectedFinishState)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_false(stream.finished());
    stream.advance(stream.size());
    test::is_true(stream.finished());
}

TEST(lexer_lexemeStream_reportsExpectedFinishState_whenOffsetFromCursor)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_false(stream.finished(2));
    test::is_true(stream.finished(4));
}

TEST(lexer_lexemeStream_reportsExpectedFinishState_whenCountIsSpecified)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_false(stream.finished(0, 3));
    test::is_true(stream.finished(2, 10));
}

TEST(lexer_lexemeStream_consumeAdvancesExpectedNumberOfLexemes_andReturnsExpectedSubStream)
{
    lexeme_stream stream(lexeme_sequence_stub());

    test::is_true(stream.peek().is(lexeme::directive, "type"));
    auto substream = stream.consume(expectation(lexeme::directive).be_true());
    test::is_true(stream.peek().is(lexeme::identifier, "FooBar"));

    test::equal(substream.size(), 1);
    test::is_true(substream.peek().is(lexeme::directive, "type"));
}

TEST(lexer_lexemeStream_advanceUpdatesCorrectly)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.peek().is(lexeme::directive, "type"));

    stream.advance();
    test::is_true(stream.peek().is(lexeme::identifier, "FooBar"));

    stream.advance(2);
    test::is_true(stream.peek().is(lexeme::string, "fubr"));
}

TEST(lexer_lexemeStream_pushSingleLexeme_addsToTemporaryBuffer)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.peek().is(lexeme::directive, "type"));

    stream.push(lexeme(50));
    test::is_true(stream.peek().is(lexeme::integer, "50"));
    test::is_true(stream.peek(1).is(lexeme::directive, "type"));
    test::equal(stream.size(), 4);
}

TEST(lexer_lexemeStream_pushMultipleLexemes_overwritesTemporaryBuffer)
{
    lexeme_stream stream(lexeme_sequence_stub());
    stream.push(lexeme(50));
    test::is_true(stream.peek(0).is(lexeme::integer, "50"));

    stream.push({ lexeme(10), lexeme(20) });
    test::is_true(stream.peek(0).is(lexeme::integer, "10"));
    test::is_true(stream.peek(1).is(lexeme::integer, "20"));
}

TEST(lexer_lexemeStream_advanceClearsTemporaryBuffer)
{
    lexeme_stream stream(lexeme_sequence_stub());

    stream.push({ lexeme(10), lexeme(20) });
    test::is_true(stream.peek(0).is(lexeme::integer, "10"));
    test::is_true(stream.peek(1).is(lexeme::integer, "20"));

    stream.advance(1);
    test::is_false(stream.peek(0).is(lexeme::integer, "10"));
    test::is_false(stream.peek(1).is(lexeme::integer, "20"));
}

TEST(lexer_lexemeStream_clearPushedLexemesUpdatesCorrectly)
{
    lexeme_stream stream(lexeme_sequence_stub());

    stream.push({ lexeme(10), lexeme(20) });
    test::is_true(stream.peek(0).is(lexeme::integer, "10"));
    test::is_true(stream.peek(1).is(lexeme::integer, "20"));

    stream.clear_pushed_lexemes();
    test::is_false(stream.peek(0).is(lexeme::integer, "10"));
    test::is_false(stream.peek(1).is(lexeme::integer, "20"));
}

TEST(lexer_lexemeStream_peekReturnsExpectedLexeme)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.peek().is(lexeme::directive, "type"));
}

TEST(lexer_lexemeStream_peekReturnsExpectedLexeme_whenOffsetGiven)
{
    lexeme_stream stream(lexeme_sequence_stub());

    stream.push({ lexeme(10), lexeme(20) });
    test::is_true(stream.peek().is(lexeme::integer, "10"));
    test::is_true(stream.peek(2).is(lexeme::directive, "type"));
    test::is_true(stream.peek(3).is(lexeme::identifier, "FooBar"));
}

TEST(lexer_lexemeStream_readReturnsExpectedLexeme)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.read().is(lexeme::directive, "type"));
    test::is_true(stream.read().is(lexeme::identifier, "FooBar"));
}

TEST(lexer_lexemeStream_readReturnsExpectedLexeme_whenOffsetGiven)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::is_true(stream.read(2).is(lexeme::colon, ":"));
    test::is_true(stream.read().is(lexeme::string, "fubr"));
}

TEST(lexer_lexemeStream_expect_correctlyMatchesAgainstSequenceOfLexemes)
{
    lexeme_stream stream(lexeme_sequence_stub());

    // 1. Match the first lexeme, purely by type.
    test::is_true(stream.expect({
        expectation(lexeme::directive).be_true()
    }));

    // 2. Match the first two lexemes, purely by type.
    test::is_true(stream.expect({
        expectation(lexeme::directive).be_true(),
        expectation(lexeme::identifier).be_true()
    }));

    // 3. Match the first lexeme by both type and string.
    test::is_true(stream.expect({
        expectation(lexeme::directive, "type").be_true()
    }));
}

TEST(lexer_lexemeStream_expect_correctlyRejectsAgainstSequenceOfLexemes)
{
    lexeme_stream stream(lexeme_sequence_stub());

    // 1. Reject with the first expectation, purely by type.
    test::is_false(stream.expect({
        expectation(lexeme::identifier).be_true()
    }));

    // 2. Reject on the last expectation, with the first being matched.
    test::is_false(stream.expect({
        expectation(lexeme::directive).be_true(),
        expectation(lexeme::integer).be_true()
    }));
}

TEST(lexer_lexemeStream_expectAny_correctlyMatchesAgainstAnExpectation)
{
    lexeme_stream stream(lexeme_sequence_stub());

    test::is_true(stream.expect_any({
        expectation(lexeme::directive, "color").be_true(),
        expectation(lexeme::directive, "type").be_true(),
        expectation(lexeme::string, "hello").be_true()
    }));
}

TEST(lexer_lexemeStream_expectAny_correctlyRejectsAgainstAllExpectations)
{
    lexeme_stream stream(lexeme_sequence_stub());

    test::is_false(stream.expect_any({
        expectation(lexeme::directive, "color").be_true(),
        expectation(lexeme::string, "type").be_true(),
        expectation(lexeme::string, "hello").be_true()
    }));
}

TEST(lexer_lexemeStream_ensure_correctlyMatchesAgainstSequenceOfLexemes)
{
    lexeme_stream stream(lexeme_sequence_stub());

    test::does_not_throw([&] {
        stream.ensure({
            expectation(lexeme::directive, "type").be_true()
        });
    });
}

TEST(lexer_lexemeStream_ensure_correctlyRejectsAgainstSequenceOfLexemes_byThrowing)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::does_throw<kdl::unexpected_lexeme_exception>([&] {
        stream.ensure({
            expectation(lexeme::identifier, "type").be_true()
        });
    });
}

TEST(lexer_lexemeStream_insertLexemes_addsLexemesToStreamPermantly_atOffset)
{
    lexeme_stream stream(lexeme_sequence_stub());
    test::equal(stream.size(), 4);

    stream.insert(std::vector<lexeme>({ lexeme(20), lexeme(10) }), 0);
    test::equal(stream.size(), 6);
    test::is_true(stream.peek().is(lexeme::integer, "20"));
    test::is_true(stream.peek(1).is(lexeme::integer, "10"));

    stream.insert(std::vector<lexeme>({ lexeme(140), lexeme(190) }), 3);
    test::equal(stream.size(), 8);
    test::is_true(stream.peek(3).is(lexeme::integer, "140"));
    test::is_true(stream.peek(4).is(lexeme::integer, "190"));
}

TEST(lexer_lexemeStream_import_addsLexemesFromLexicalOutput_usingAnotherSourceFile)
{

}
