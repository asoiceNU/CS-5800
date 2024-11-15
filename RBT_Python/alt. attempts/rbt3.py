class Node:
    def __init__(self, data):
        self.data = data
        self.color = 'RED'  # New nodes are always red initially
        self.left = None
        self.right = None
        self.parent = None

class RedBlackTree:
    def __init__(self):
        self.TNULL = Node(0)  # Sentinel node used for leaves
        self.TNULL.color = 'BLACK'
        self.root = self.TNULL

    def left_rotate(self, x):
        y = x.right
        x.right = y.left
        if y.left != self.TNULL:
            y.left.parent = x
        y.parent = x.parent
        if x.parent is None:
            self.root = y
        elif x == x.parent.left:
            x.parent.left = y
        else:
            x.parent.right = y
        y.left = x
        x.parent = y

    def right_rotate(self, x):
        y = x.left
        x.left = y.right
        if y.right != self.TNULL:
            y.right.parent = x
        y.parent = x.parent
        if x.parent is None:
            self.root = y
        elif x == x.parent.right:
            x.parent.right = y
        else:
            x.parent.left = y
        y.right = x
        x.parent = y

    def fix_insert(self, k):
        while k.parent.color == 'RED':
            if k.parent == k.parent.parent.left:
                u = k.parent.parent.right  # Uncle
                if u.color == 'RED':
                    # Case 1: Uncle is red
                    k.parent.color = 'BLACK'
                    u.color = 'BLACK'
                    k.parent.parent.color = 'RED'
                    k = k.parent.parent
                else:
                    if k == k.parent.right:
                        # Case 2: k is the right child
                        k = k.parent
                        self.left_rotate(k)
                    # Case 3: k is the left child
                    k.parent.color = 'BLACK'
                    k.parent.parent.color = 'RED'
                    self.right_rotate(k.parent.parent)
            else:
                u = k.parent.parent.left  # Uncle
                if u.color == 'RED':
                    # Case 1: Uncle is red
                    k.parent.color = 'BLACK'
                    u.color = 'BLACK'
                    k.parent.parent.color = 'RED'
                    k = k.parent.parent
                else:
                    if k == k.parent.left:
                        # Case 2: k is the left child
                        k = k.parent
                        self.right_rotate(k)
                    # Case 3: k is the right child
                    k.parent.color = 'BLACK'
                    k.parent.parent.color = 'RED'
                    self.left_rotate(k.parent.parent)
            if k == self.root:
                break
        self.root.color = 'BLACK'

    def insert(self, key):
        node = Node(key)
        node.parent = None
        node.data = key
        node.left = self.TNULL
        node.right = self.TNULL
        node.color = 'RED'

        y = None
        x = self.root

        while x != self.TNULL:
            y = x
            if node.data < x.data:
                x = x.left
            else:
                x = x.right

        node.parent = y
        if y is None:
            self.root = node
        elif node.data < y.data:
            y.left = node
        else:
            y.right = node

        if node.parent is None:
            node.color = 'BLACK'
            return

        if node.parent.parent is None:
            return

        self.fix_insert(node)

    def search_tree(self, node, key):
        if node == self.TNULL or key == node.data:
            return node
        if key < node.data:
            return self.search_tree(node.left, key)
        return self.search_tree(node.right, key)

    def minimum(self, node):
        while node.left != self.TNULL:
            node = node.left
        return node

    def maximum(self, node):
        while node.right != self.TNULL:
            node = node.right
        return node

    def successor(self, x):
        if x.right != self.TNULL:
            return self.minimum(x.right)
        y = x.parent
        while y != self.TNULL and x == y.right:
            x = y
            y = y.parent
        return y

    def predecessor(self, x):
        if x.left != self.TNULL:
            return self.maximum(x.left)
        y = x.parent
        while y != self.TNULL and x == y.left:
            x = y
            y = y.parent
        return y

    def inorder(self, node):
        if node != self.TNULL:
            self.inorder(node.left)
            print(node.data, end=" ")
            self.inorder(node.right)

    def print_tree(self):
        self.inorder(self.root)
        print()

    def tree_height(self, node):
        if node == self.TNULL:
            return 0
        else:
            left_height = self.tree_height(node.left)
            right_height = self.tree_height(node.right)
            return max(left_height, right_height) + 1
