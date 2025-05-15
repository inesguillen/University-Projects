
import unittest
from phase1 import SList2

class Test(unittest.TestCase):

    #setUp is a method which is ran before a test method is executed.
   	#This is useful if you need some data (for example) to be present before running a test.
    def setUp(self):
        pass
      
    #implement here your test cases
    def test_delLargestSeq_largestSeq(self):  
      print("\nlaunching test_delLargestSeq_largestSeq")
      seq = [3,3,3,4,5,6,6,6,7,7,7,7,2]
      expected = [3,3,3,4,5,6,6,6,2]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_delLargestSeq_lastlargestSeq(self):  
      print("\nlaunching test_delLargestSeq_lastlargestSeq")
      seq = [8,8,8,8,4,5,6,6,6,7,7,7,7,2]
      expected = [8,8,8,8,4,5,6,6,6,2]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_delLargestSeq_noLargest(self):
      print("\nlaunching test_delLargestSeq_noLargest")
      seq = [6,6,8,8,4,4,12,12]
      expected =[6,6,8,8,4,4]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_delLargestSeq_noLargest1(self):
      print("\nlaunching test_delLargestSeq_noLargest1")
      seq = [1,2,3,4,5]
      expected =[1,2,3,4]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_delLargestSeq_UniqueSeq(self):
      print("\nlaunching test_delLargestSeq_UniqueSeq")
      seq = [10]
      expected =[]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_delLargestSeq_EmptySeq(self):
      print("\nlaunching test_delLargestSeq_EmptySeq")
      seq = []
      expected =[]
      lList = SList2ToList(seq, SList2.delLargestSeq)
      self.assertListEqual(lList, expected)

    def test_fix_loopSeqFalse(self):
      print("\nlaunching test_fix_loopSeqFalse")
      l=SList2()
      l.addLast("X")
      l.addLast("Y")
      l.addLast("Z")
      res= l.fix_loop()
      self.assertEqual(res, False)

    def test_fix_loopSeqTrueComplete(self):
      print("\nlaunching test_fix_loopSeqTrueComplete")
      l=SList2()
      l.addLast("X")
      l.addLast("Y")
      l.addLast("Z")
      l._head.next.next.next=l._head # A B C A B C ...
      res= l.fix_loop()
      self.assertEqual(res, True)
      #self.assertEqual(l._head.next.next.next, None)

    def test_fix_loopSeqTruePartial(self):
      print("\nlaunching test_fix_loopSeqTruePartial")
      l=SList2()
      l.addLast("X")
      l.addLast("Y")
      l.addLast("Z")
      l._head.next.next.next=l._head.next # A B C B C ...
      res= l.fix_loop()
      self.assertEqual(res, True)
      #self.assertEqual(l._head.next.next.next, None)

    def test_leftrightShift_Left(self):
      print("\nlaunching test_leftrightShift_Left")
      seq=[1,2,3,4,5,6,7,8]
      expected=[5,6,7,8,1,2,3,4]
      l = SList2()
      for e in seq:
          l.addLast(e)
      l.leftrightShift(True,4)
      lList = [l.getAt(i) for i in range(0, len(l))]
      self.assertListEqual(lList, expected)

    def test_leftrightShift_Right(self):
      print("\nlaunching test_leftrightShift_Right")
      seq=[1,2,3,4,5,6,7,8]
      expected=[7,8,1,2,3,4,5,6]
      l = SList2()
      for e in seq:
          l.addLast(e)
      l.leftrightShift(False,2)
      lList = [l.getAt(i) for i in range(0, len(l))]
      self.assertListEqual(lList, expected)


def SList2ToList(seq, fun): # Initializes a SList2 with a list, performs operation and returns the result 
    l=SList2()
    for e in seq:
        l.addLast(e)
    fun(l)
    return [l.getAt(i) for i in range(0,len(l))]


if __name__ == "__main__":
    unittest.main()