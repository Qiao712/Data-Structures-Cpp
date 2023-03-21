import java.util.List;

public class BPlusTree implements List {
    /**
     * B+树的阶数
     */
    private final int m;

    private static class Node {
        static enum NodeType {

        }

        Node[] children;
    }

    public BPlusTree(int m) {
        this.m = m;
    }

}