from slistH import SList
from slistH import SNode

import sys

class SList2(SList):

    def delLargestSeq(self):
        '''This method deletes all the elements of the largest sequence of equal numbers from the calling list'''
        #example: [3,3,3,4,3,5,6,6,6,7,7,7,7,2]

        # when list is already empty
        if len(self)==0:
            return

        result = 1                          # number of repetitions of said number
        max_res = 0                         # largest seq of reps at the moment
        max_ind = 0                         # final first index with largest seq
        last = self.getAt(0)                # last number of the largest seq

        for i in range(1,len(self)):
            if self.getAt(i) == last:
                result += 1  #4
            else:
                if result >= max_res:
                    max_res = result  #1
                    max_ind = i - result   #8-1=7
                last = self.getAt(i)  #7
                result = 1   #1

        # this will also be outside the loop bc we have to update the variables in case we enter the first if
        if result >= max_res:
            max_res = result  #2
            max_ind = len(self) - result   #13-4=9
        # we delete the element at max_index, max_res times (we don't move as the elements will keep updating to the max_ind)
        for i in range(0, max_res):
            self.removeAt(max_ind)


    def fix_loop(self):
        #implement here your solution
        '''We use two counters in order to prove if there exists a loop or not'''
        slow_c = self._head
        fast_c = self._head

        while slow_c and fast_c and fast_c.next:
            slow_c = slow_c.next
            fast_c = fast_c.next.next
            # one counter is faster than the other one, so if they get back to the same node, it means there exists a loop
            if slow_c == fast_c:
                return 1
        return 0


    def create_loop(self, position):
        # this method is used to force a loop in a singly linked list
        if position < 0 or position > len(self) - 1:
            raise ValueError(f"Position out of range [{0} - {len(self) - 1}]")

        current = self._head
        i = 0

        # We reach position to save the reference
        while current and i < position:
            current = current.next
            i += 1

        # We reach to tail node and set the loop
        start_node = current
        print(f"Creating a loop starting from {start_node.elem}")
        while current.next:
            current = current.next
        current.next = start_node


    def leftrightShift(self,left,n):
        #implement here your solution
        if len(self)==0 or n>=len(self):    # the list is already empty or n is greater than the size
            return

        if left:                            #we add at the end of the list
            for i in range(n):
                elem=self.removeFirst()
                self.addLast(elem)

        if not left:                        #we add at the beginning
            for i in range(n):
                elem=self.removeLast()
                self.addFirst(elem)


                #[2,3,5,6,4,3]   2  [4,3,2,3,5,6]


if __name__ == '__main__':

    l = SList2()
    print("list:", str(l))
    print("len:", len(l))

    for i in range(7):
        l.addLast(i + 1)

    print(l)
    print()

    l = SList2()
    print("list:", str(l))
    print("len:", len(l))

    for i in range(7):
        l.addLast(i + 1)

    print(l)
    print()

    # No loop yet, no changes applied
    l.fix_loop()
    print("No loop yet, no changes applied")
    print(l)
    print()

    # We force a loop
    l.create_loop(position=6)
    l.fix_loop()
    print("Loop fixed, changes applied")
    print(l)
    print()
    print()

    l = SList2()
    for i in [1, 2, 3, 4, 5]:
        l.addLast(i)
    print(l.delLargestSeq())

    l = SList2()
    for i in range(7):
        l.addLast(i + 1)

    print(l)
    l.leftrightShift(False, 2)
    print(l)

