# Copyright (c) 2023 Tom Hancocks
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

test_suite(KDL)

    test_case(Lexeme)
        test(lexer_lexeme_constructStringValueLexemeCorrectly)
        test(lexer_lexeme_constructIntegerLexemeCorrectly)
        test(lexer_lexeme_constructBasicLexemeCorrectly)
        test(lexer_lexeme_constructComponentsLexemeCorrectly)
        test(lexer_lexeme_isLexeme_reportsCorrectly)
        test(lexer_lexeme_isLexemeType_reportsCorrectly)
        test(lexer_lexeme_isLexemeString_reportsCorrectly)
        test(lexer_lexeme_isLexemeTypeString_reportsCorrectly)
        test(lexer_lexeme_reportsBasicTextCorrectly)
        test(lexer_lexeme_reportsComponentsTextCorrectly)
        test(lexer_lexeme_reportsComponentsCorrectly)
        test(lexer_lexeme_reportsIntegerValueCorrectly)
        test(lexer_lexeme_reportsLeftAssociativityCorrectly)
    end_test_case()

    test_case(LexemeStream)
        test(lexer_lexemeStream_constructedCorrectly)
        test(lexer_lexemeStream_reportsSizeCorrectly)
        test(lexer_lexemeStream_returnsExpectedLexemeAtIndex)
        test(lexer_lexemeStream_reportsExpectedFinishState)
        test(lexer_lexemeStream_reportsExpectedFinishState_whenOffsetFromCursor)
        test(lexer_lexemeStream_reportsExpectedFinishState_whenCountIsSpecified)
        test(lexer_lexemeStream_consumeAdvancesExpectedNumberOfLexemes_andReturnsExpectedSubStream)
        test(lexer_lexemeStream_advanceUpdatesCorrectly)
        test(lexer_lexemeStream_pushSingleLexeme_addsToTemporaryBuffer)
        test(lexer_lexemeStream_pushMultipleLexemes_overwritesTemporaryBuffer)
        test(lexer_lexemeStream_advanceClearsTemporaryBuffer)
        test(lexer_lexemeStream_clearPushedLexemesUpdatesCorrectly)
        test(lexer_lexemeStream_peekReturnsExpectedLexeme)
        test(lexer_lexemeStream_peekReturnsExpectedLexeme_whenOffsetGiven)
        test(lexer_lexemeStream_readReturnsExpectedLexeme)
        test(lexer_lexemeStream_readReturnsExpectedLexeme_whenOffsetGiven)
        test(lexer_lexemeStream_expect_correctlyMatchesAgainstSequenceOfLexemes)
        test(lexer_lexemeStream_expect_correctlyRejectsAgainstSequenceOfLexemes)
        test(lexer_lexemeStream_expectAny_correctlyMatchesAgainstAnExpectation)
        test(lexer_lexemeStream_expectAny_correctlyRejectsAgainstAllExpectations)
        test(lexer_lexemeStream_ensure_correctlyMatchesAgainstSequenceOfLexemes)
        test(lexer_lexemeStream_ensure_correctlyRejectsAgainstSequenceOfLexemes_byThrowing)
        test(lexer_lexemeStream_insertLexemes_addsLexemesToStreamPermantly_atOffset)
        test(lexer_lexemeStream_import_addsLexemesFromLexicalOutput_usingAnotherSourceFile)
    end_test_case()

    test_case(Lexer)

    end_test_case()

    test_case(LexemeExpectation)
        test(lexer_expectation_expectType_toBeTrue)
        test(lexer_expectation_expectType_toBeFalse)
        test(lexer_expectation_expectValue_toBeTrue)
        test(lexer_expectation_expectValue_toBeFalse)
        test(lexer_expectation_expectTypeAndValue_toBeTrue)
        test(lexer_expectation_expectTypeAndValue_toBeFalse)
    end_test_case()

    test_case(LexerConditionDecimalSet)
        test(lexer_condition_decimalSet_containsDecimalNumerals)
        test(lexer_condition_decimalSet_doesNotContainUnexpectedCharacters)
    end_test_case()

    test_case(LexerConditionHexadecimalSet)
        test(lexer_condition_hexadecimalSet_containsDecimalNumerals)
        test(lexer_condition_hexadecimalSet_containsHexNumerals)
        test(lexer_condition_hexadecimalSet_doesNotContainUnexpectedCharacters)
    end_test_case()

    test_case(LexerConditionIdentifierSet)
        test(lexer_condition_identifierSet_limitedContainsDoesNotContainDecimalNumerals)
        test(lexer_condition_identifierSet_containsDecimalNumerals)
        test(lexer_condition_identifierSet_doesNotContainUnexpectedCharacters)
    end_test_case()

    test_case(LexerConditionMatch)
        test(lexer_condition_matchTemplate_yes_returnsTrueIfEqual)
        test(lexer_condition_matchTemplate_yes_returnsFalseIfNotEqual)
        test(lexer_condition_matchTemplate_no_returnsFalseIfEqual)
        test(lexer_condition_matchTemplate_no_returnsTrueIfNotEqual)
    end_test_case()

    test_case(LexerConditionRange)
        test(lexer_condition_rangeTemplate_containsExpectedCharacter)
        test(lexer_condition_rangeTemplate_notContainsUnexpectedCharacter)
    end_test_case()

    test_case(LexerConditionSequence)
        test(lexer_condition_sequenceTemplate_yes_returnsTrueIfStringContainsCharacterSequence)
        test(lexer_condition_sequenceTemplate_no_returnsTrueIfStringDoesNotContainCharacterSequence)
    end_test_case()

    test_case(LexerConditionSet)
        test(lexer_condition_setTemplate_containsExpectedCharacter)
        test(lexer_condition_setTemplate_notContainsUnexpectedCharacter)
    end_test_case()

end_test_suite()