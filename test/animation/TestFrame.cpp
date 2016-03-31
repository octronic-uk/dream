/*
* Dream::Animation::Test::TestFrame
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TestFrame.h"

#include "../../src/animation/Frame.h"
#include "../../src/animation/FrameDelta.h"

namespace Dream {
	namespace Animation {
		namespace Test {
			void TestFrame::run() {
				testHeading("DreamAnimation::Frame");
				testFrameCreate();
				testFrameAddFrameDelta();
				testFrameGetNextAvailableFrameDeltaIndex();
				testFrameCompareIndicies();
				testFrameGetNumFrameDeltas ();
			}

			void TestFrame::testFrameCreate(void) {
				printComment("Create Frame");
				Dream::Animation::Frame *frame = new Dream::Animation::Frame(0);
				assertNotNull("Frame Constructor", frame);
				return;
			}

			void TestFrame::testFrameAddFrameDelta(void) {
				Dream::Animation::Frame *frame = new Dream::Animation::Frame(0);
				Dream::Animation::FrameDelta *delta1 = new Dream::Animation::FrameDelta(0,DA_OP_LINEAR);
				Dream::Animation::FrameDelta *delta2 = new Dream::Animation::FrameDelta(0,DA_OP_LINEAR);
				frame->addFrameDelta(delta1);
				frame->addFrameDelta(delta2);
				assertEqual("Number of deltas in frame is 2", frame->getNumFrameDeltas(),2);
				return;
			}
			void TestFrame::testFrameGetNextAvailableFrameDeltaIndex(void){
				Dream::Animation::Frame* frame = new Dream::Animation::Frame(0);

				assertEqual(
					"Get Next Available Frame Delta Index of empty frame is 0",
					frame->getNextAvailableFrameDeltaIndex(),
					0
				);

				Dream::Animation::FrameDelta* delta = new Dream::Animation::FrameDelta(0,DA_OP_LINEAR);
				frame->addFrameDelta(delta);
				assertEqual(
					"Get Next Available Frame Delta Index of frame with 1 delta is 1",
					frame->getNextAvailableFrameDeltaIndex(),
					1
				);
				return;
			}

			void TestFrame::testFrameCompareIndicies(void) {
				Dream::Animation::Frame *frame1;
				Dream::Animation::Frame *frame2;

				frame1 = new Dream::Animation::Frame(100);
				frame2 = new Dream::Animation::Frame(200);

				int result1 = frame1->compareIndecies(frame2);
				int result2 = frame2->compareIndecies(frame1);

				assertEqual("Frame Index Comparison",result1,100);
				assertEqual("Frame Index Comparison",result2,-100);
				return;
			}

			void TestFrame::testFrameGetNumFrameDeltas(void) {
				Dream::Animation::Frame* frame = new Dream::Animation::Frame(0);

				Dream::Animation::FrameDelta *delta1, *delta2;
				delta1 = new Dream::Animation::FrameDelta(0,0);
				frame->addFrameDelta(delta1);
				assertEqual("Get Num Frame Deltas == 1",frame->getNumFrameDeltas(),1);

				delta2 = new Dream::Animation::FrameDelta(0,0);
				frame->addFrameDelta(delta2);
				assertEqual("Get Num Frame Deltas == 2",frame->getNumFrameDeltas(),2);
				return;
			}
		}
	}
}