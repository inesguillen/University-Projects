# -*- coding: utf-8 -*-
"""
Test program comparing solutions with the builtin list-based one.

@author: EDA Team
"""

# Classes provided by EDA Team
from bst import BinarySearchTree
import unittest
from dlist import DList
from phase2 import BST2

class Test(unittest.TestCase):
    def setUp(self):
        self.emptyTree = BST2()
        self.secTree = BST2()
        self.secTree.insert(14)
        self.secTree.insert(11)
        self.secTree.insert(18)
        self.secTree.insert(10)
        self.secTree.insert(13)
        self.secTree.insert(16)
        self.secTree.insert(19)
        self.secTree.insert(39)
        self.secTree.insert(52)
        self.btree = BST2()
        self.btree.insert(14)
        self.btree.insert(11)
        self.btree.insert(18)
        self.btree.insert(10)
        self.btree.insert(13)
        self.btree.insert(16)
        self.btree.insert(19)
        self.btree.insert(5)
        self.btree.insert(12)
        self.btree.insert(15)
        self.btree.insert(17)
        self.btree.insert(30)
        self.btree.insert(4)
        self.btree.insert(6)
        self.btree.insert(29)
        self.btree.insert(31)
        self.btree.insert(2)
        self.btree.insert(8)
        self.btree.insert(24)
        self.btree.insert(33)
        self.btree.insert(1)
        self.btree.insert(3)
        self.btree.insert(7)
        self.btree.insert(9)
        self.btree.insert(23)
        self.btree.insert(25)
        self.btree.insert(32)
        self.btree.insert(34)
        self.btree.insert(21)
        self.btree.insert(27)
        self.btree.insert(36)
        self.btree.insert(20)
        self.btree.insert(22)
        self.btree.insert(26)
        self.btree.insert(28)
        self.btree.insert(35)
        self.btree.insert(37)

    def test_find_dist_k_test01(self):
        """ n doesn't exist in the tree """
        nodeList = self.btree.find_dist_k(60, 0)
        self.assertTrue(nodeList.is_empty())

    def test_find_dist_k_test02(self):
        """ k is a negative number (incorrect) """
        nodeList = self.btree.find_dist_k(10, -4)
        expected = DList()
        #### an exception is expected, we need to test that an exception is
        #### thrown because it's designed to throw the exception for negative
        #### numbers
        self.assertEqual(nodeList, expected)

    def test_find_dist_k_test03(self):
        """ k is 0 (base node) """
        nodeList = self.btree.find_dist_k(30, 0)
        expected = DList()
        expected.add_first(30)
        self.assertEqual(nodeList.remove_last(), expected.remove_last())

    def test_find_dist_k_test04(self):
        """ k is 2 """
        nodeList = self.btree.find_dist_k(30, 2)
        expected = DList()
        expected.add_last(24)
        expected.add_last(33)
        self.assertEqual(nodeList.remove_last(), expected.remove_last())
        #self.assertListEqual(nodeList, [18, 24, 33])

    def test_find_dist_k_test05(self):
        """ k > 2 """
        nodeList = self.btree.find_dist_k(12, 6)
        expected = DList()
        expected.add_last(2)
        expected.add_last(8)
        expected.add_last(15)
        expected.add_last(17)
        expected.add_last(30)
        self.assertEqual(nodeList, expected)
        #self.assertEqual(nodeList.remove_last(), expected.remove_last())

    def test_create_tree00(self):
        """ operations between an empty tree and a secondary tree """
        newMergedTree = self.emptyTree.create_tree(self.emptyTree, self.secTree, 'merge')
        newIntersTree = self.emptyTree.create_tree(self.emptyTree, self.secTree, 'intersection')
        newDiffTree = self.emptyTree.create_tree(self.secTree, self.emptyTree, 'difference')
        self.assertEqual(9, newMergedTree.size())
        self.assertEqual(0, newIntersTree.size())
        self.assertEqual(9, newDiffTree.size())

    def test_create_tree01(self):
        """ operations between the main tree and a secondary tree """
        newMergedTree = self.btree.create_tree(self.btree, self.secTree, 'merge')
        newIntersTree = self.btree.create_tree(self.btree, self.secTree, 'intersection')
        newDiffTree = self.btree.create_tree(self.secTree, self.btree, 'difference')
        self.assertEqual(self.btree.size(), newMergedTree.size())
        self.assertEqual(self.btree.size(), newIntersTree.size())
        self.assertEqual(2, newDiffTree.size())

    def test_create_tree02(self):
        """ operations between an empty tree and the main tree """
        newMergedTree = self.btree.create_tree(self.emptyTree, self.btree, 'merge')
        newIntersTree = self.btree.create_tree(self.emptyTree, self.btree, 'intersection')
        newDiffTree = self.btree.create_tree(self.btree, self.emptyTree, 'difference')
        self.assertEqual(self.btree.size(), newMergedTree.size())
        self.assertEqual(0, newIntersTree.size())
        self.assertEqual(self.btree.size(), newDiffTree.size())


# Some usage examples
if __name__ == '__main__':
    unittest.main()
