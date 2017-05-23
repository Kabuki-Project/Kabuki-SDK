/** Kabuki Theater
    @file    /.../Source/KabukiTheater/_VEnv/TESTS/Workspace/main.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#include <_Theater.hpp>
#include <CppUTest\CommandLineTestRunner.h>
#include <CppUTest\TestHarness.h>
#include <stdio.h>

using namespace _Theater;

TEST_GROUP (_ProjectTests)
{
    void setup ()
    {
        // Init stuff
    }

    void teardown ()
    {
        printf ("\n\n_Theater::ProjectTests completed.\n\n");
        //system("PAUSE");
    }
};

TEST (_ProjectTests, ProjectTests1)
{
}
