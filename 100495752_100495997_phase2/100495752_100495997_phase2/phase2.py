"""
@author: EDA Team
"""

# Classes provided by EDA Team
from bintree import BinaryNode
from bst import BinarySearchTree
from dlist import DList


# Exercise #1
class BST2(BinarySearchTree):
    def find_dist_k(self, n: int, k: int):
        # Here your code
        result = DList()
        node = self.search(n)
        if node is None:
            return result
        if k < 0:
            print("k must be positive")
            #raise ValueError("k must be positive")
            return result

        self._find_dist_k(node, k, result)
        return result

    def _find_dist_k(self, node, k, result):
        if node is None:
            return
        if k == 0:
            result.add_last(node.elem)
        else:
            self._find_dist_k(node.left, k - 1, result)
            self._find_dist_k(node.right, k - 1, result)

    # Exercise #2
    def create_tree(self, input_tree1: BinarySearchTree,
                    input_tree2: BinarySearchTree,
                    opc: str) -> BinarySearchTree:
        a = input_tree1
        b = input_tree2
        list_nodes = DList()
        if opc == "merge":
            list_nodes.add_last(b._root)
            if b._root is None:
                return a
            elif a._root is None:
                return b
            while len(list_nodes) > 0:
                current = list_nodes.remove_first()
                nodeInA = a.search(current.elem)
                if nodeInA is None:
                    a.insert(current.elem)
                if current.left is not None:
                    list_nodes.add_last(current.left)  # O(1)
                if current.right is not None:
                    list_nodes.add_last(current.right)  # O(1)
            #print(a)
            return a

        elif opc == "intersection":
            list_nodes.add_last(a._root)
            if b._root is None or a._root is None:
                return BST2()
            while len(list_nodes) > 0:
                current = list_nodes.remove_first()
                nodeInB = b.search(current.elem)
                if nodeInB is None:
                    a.remove(current.elem)
                if current.left is not None:
                    list_nodes.add_last(current.left)
                if current.right is not None:
                    list_nodes.add_last(current.right)
            #print(a)
            return a

        elif opc == "difference":
            list_nodes.add_last(a._root)
            if b._root is None or a._root is None:
                return a
            while len(list_nodes) > 0:
                current = list_nodes.remove_first()
                nodeInB = b.search(current.elem)
                if nodeInB is not None:
                    a.remove(current.elem)
                if current.left is not None:
                    list_nodes.add_last(current.left)
                if current.right is not None:
                    list_nodes.add_last(current.right)
            #print(a)
            return a


# Some usage examples
if __name__ == '__main__':
    # input_list_01 = [5, 1, 7, 9, 23]
    # input_list_02 = [1, 9, 11]
    input_list_01 = [5, 12, 2, 1, 3, 9]
    input_list_02 = [9, 3, 21]

    # Build and draw first tree
    ###changed tree one from BinarySearchTree to BST2
    tree1 = BST2()
    for x in input_list_01:
        tree1.insert(x)
    tree1.draw()

    # Build and draw second tree
    tree2 = BinarySearchTree()
    for x in input_list_02:
        tree2.insert(x)
    tree2.draw()

    function_names = ["merge", "intersection", "difference"]

    for op_name in function_names:
        res = tree1.create_tree(tree1, tree2, op_name)
        print(f"-- Result for {op_name} method. #{res.size()} nodes")
        res.draw()
