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

test_suite(ECS)
    test_case(StoragePool)
        test(acquire_returnsReferencesWithExpectedValues)
        test(destroyReference_releasesAcquiredValueBackToPool)
        test(acquire_expandsPoolWhenDepleted)
        test(capacity_reportsExpectedValue)
    end_test_case()
    test_case(Storage)
        test(createEntity_returnsValidStorageLink)
        test(getEntity_returnsValidStorageLink)
        test(subscript_returnsValidStorageLink)
        test(ephemeralGet_returnsValidEntityReference)
        test(reacquiringEntity_correctlyResetsPreviousValues)
    end_test_case()
    test_case(EntityIteration)
        test(iterate_onlyVisitsCreatedEntities)
        test(iterate_skipDestroyedEntities)
    end_test_case()
end_test_suite()
