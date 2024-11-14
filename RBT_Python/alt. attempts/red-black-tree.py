class Node:
    def __init__(self, data):
        self.data = data
        self.color = "RED"  # All new nodes are initially red
        self.left = None
        self.right = None
        self.parent = None


class RedBlackTree:
    def __init__(self):
        self.TNULL = Node(0)  # Sentinel node (TNULL) to represent NULL leaves
        self.TNULL.color = "BLACK"
        self.root = self.TNULL  # Initially, the tree is empty

    # Left rotate
    def left_rotate(self, x):
        y = x.right
        x.right = y.left
        if y.left != self.TNULL:
            y.left.parent = x
        y.parent = x.parent
        if x.parent == None:
            self.root = y
        elif x == x.parent.left:
            x.parent.left = y
        else:
            x.parent.right = y
        y.left = x
        x.parent = y

    # Right rotate
    def right_rotate(self, x):
        y = x.left
        x.left = y.right
        if y.right != self.TNULL:
            y.right.parent = x
        y.parent = x.parent
        if x.parent == None:
            self.root = y
        elif x == x.parent.right:
            x.parent.right = y
        else:
            x.parent.left = y
        y.right = x
        x.parent = y

    # Insert a node into the tree
    def insert(self, key):
        node = Node(key)
        node.parent = None
        node.data = key
        node.left = self.TNULL
        node.right = self.TNULL
        node.color = "RED"

        y = None
        x = self.root

        # Standard BST insertion
        while x != self.TNULL:
            y = x
            if node.data < x.data:
                x = x.left
            else:
                x = x.right

        node.parent = y
        if y == None:
            self.root = node
        elif node.data < y.data:
            y.left = node
        else:
            y.right = node

        if node.parent == None:
            node.color = "BLACK"
            return

        if node.parent.parent == None:
            return

        # Fix the red-black tree properties after insertion
        self.fix_insert(node)

    # Fix the red-black tree properties after insertion
    def fix_insert(self, k):
        while k.parent.color == "RED":
            if k.parent == k.parent.parent.left:
                u = k.parent.parent.right  # Uncle
                if u.color == "RED":
                    # Case 1: Uncle is red
                    k.parent.color = "BLACK"
                    u.color = "BLACK"
                    k.parent.parent.color = "RED"
                    k = k.parent.parent
                else:
                    if k == k.parent.right:
                        # Case 2: k is the right child
                        k = k.parent
                        self.left_rotate(k)
                    # Case 3: k is the left child
                    k.parent.color = "BLACK"
                    k.parent.parent.color = "RED"
                    self.right_rotate(k.parent.parent)
            else:
                u = k.parent.parent.left  # Uncle
                if u.color == "RED":
                    # Case 1: Uncle is red
                    k.parent.color = "BLACK"
                    u.color = "BLACK"
                    k.parent.parent.color = "RED"
                    k = k.parent.parent
                else:
                    if k == k.parent.left:
                        # Case 2: k is the left child
                        k = k.parent
                        self.right_rotate(k)
                    # Case 3: k is the right child
                    k.parent.color = "BLACK"
                    k.parent.parent.color = "RED"
                    self.left_rotate(k.parent.parent)
            if k == self.root:
                break
        self.root.color = "BLACK"

    # In-order traversal (to print the sorted elements)
    def inorder(self, node, result):
        if node != self.TNULL:
            self.inorder(node.left, result)
            result.append(node.data)
            self.inorder(node.right, result)

    def sort(self):
        result = []
        self.inorder(self.root, result)
        return result

    # Search for a node with a given key
    def search(self, node, key):
        if node == self.TNULL or key == node.data:
            return node
        if key < node.data:
            return self.search(node.left, key)
        return self.search(node.right, key)

    # Find the minimum node
    def minimum(self, node):
        while node.left != self.TNULL:
            node = node.left
        return node

    # Find the maximum node
    def maximum(self, node):
        while node.right != self.TNULL:
            node = node.right
        return node

    # Find the successor of a given node
    def successor(self, x):
        if x.right != self.TNULL:
            return self.minimum(x.right)
        y = x.parent
        while y != self.TNULL and x == y.right:
            x = y
            y = y.parent
        return y

    # Find the predecessor of a given node
    def predecessor(self, x):
        if x.left != self.TNULL:
            return self.maximum(x.left)
        y = x.parent
        while y != self.TNULL and x == y.left:
            x = y
            y = y.parent
        return y

    # Delete a node from the tree
    def delete(self, key):
        node = self.search(self.root, key)
        if node == self.TNULL:
            return

        y = node
        y_original_color = y.color
        if node.left == self.TNULL:
            x = node.right
            self.transplant(node, node.right)
        elif node.right == self.TNULL:
            x = node.left
            self.transplant(node, node.left)
        else:
            y = self.minimum(node.right)
            y_original_color = y.color
            x = y.right
            if y.parent == node:
                x.parent = y
            else:
                self.transplant(y, y.right)
                y.right = node.right
                y.right.parent = y
            self.transplant(node, y)
            y.left = node.left
            y.left.parent = y
            y.color = node.color

        if y_original_color == "BLACK":
            self.fix_delete(x)

    # Fix the red-black tree after deletion
    def fix_delete(self, x):
        while x != self.root and x.color == "BLACK":
            if x == x.parent.left:
                w = x.parent.right
                if w.color == "RED":
                    w.color = "BLACK"
                    x.parent.color = "RED"
                    self.left_rotate(x.parent)
                    w = x.parent.right
                if w.left.color == "BLACK" and w.right.color == "BLACK":
                    w.color = "RED"
                    x = x.parent
                else:
                    if w.right.color == "BLACK":
                        w.left.color = "BLACK"
                        w.color = "RED"
                        self.right_rotate(w)
                        w = x.parent.right
                    w.color = x.parent.color
                    x.parent.color = "BLACK"
                    w.right.color = "BLACK"
                    self.left_rotate(x.parent)
                    x = self.root
            else:
                w = x.parent.left
                if w.color == "RED":
                    w.color = "BLACK"
                    x.parent.color = "RED"
                    self.right_rotate(x.parent)
                    w = x.parent.left
                if w.right.color == "BLACK" and w.left.color == "BLACK":
                    w.color = "RED"
                    x = x.parent
                else:
                    if w.left.color == "BLACK":
                        w.right.color = "BLACK"
                        w.color = "RED"
                        self.left_rotate(w)
                        w = x.parent.left
                    w.color = x.parent.color
                    x.parent.color = "BLACK"
                    w.left.color = "BLACK"
                    self.right_rotate(x.parent)
                    x = self.root
        x.color = "BLACK"

    # Transplant helper function for deletion
    def transplant(self, u, v):
        if u.parent == None:
            self.root = v
        elif u == u.parent.left:
            u.parent.left = v
        else:
            u.parent.right = v
        v.parent = u.parent

    # Print the tree structure for verification (simple tree visualization)
    def print_tree(self, node, indent="", last='updown'):
        if node != self.TNULL:
            print(f"{indent}<{'R' if node.color == 'RED' else 'B'}> {node.data}")
            indent += "   " if last == 'updown' else "|  "
            self.print_tree(node.left, indent, 'left')
            self.print_tree(node.right, indent, 'right')


# Test the Red-Black Tree implementation
if __name__ == "__main__":
    rbt = RedBlackTree()

    # Inserting nodes
    insert_values = [20, 15, 25, 10, 30, 5, 12, 22, 35]
    for value in insert_values:
        rbt.insert(value)

    # Print the tree after insertion
    print("Tree after insertions:")
    rbt.print_tree(rbt.root)

    # Print sorted order (inorder traversal)
    print("\nSorted Tree (Inorder Traversal):", rbt.sort())

    # Test search
    node = rbt.search(rbt.root, 25)
    print("\nSearch result for 25:", node.data if node != rbt.TNULL else "Not found")

    # Test min and max
    min_node = rbt.minimum(rbt.root)
    max_node = rbt.maximum(rbt.root)
    print("\nMin:", min_node.data, "Max:", max_node.data)

    # Test successor and predecessor
    succ_node = rbt.successor(node)
    pred_node = rbt.predecessor(node)
    print("\nSuccessor of 25:", succ_node.data if succ_node != rbt.TNULL else "None")
    print("Predecessor of 25:", pred_node.data if pred_node != rbt.TNULL else "None")

    # Delete a node and print tree again
    print("\nDeleting 15")
    rbt.delete(15)
    rbt.print_tree(rbt.root)

    # Final inorder traversal after deletion
    print("\nSorted Tree after deletion:", rbt.sort())
