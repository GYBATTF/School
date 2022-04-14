import java.io.File;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.regex.Pattern;

/**
 * Basic database?
 * @author Alexander Harms
 * ajharms
 */
public class Toy {
    /**
     * Where to get input from
     */
    private static final Scanner INPUT = new Scanner(System.in);

    /**
     * Main method
     * @param args
     * program arguments, first one being the operation to perform
     * @throws Exception
     * all sorts of reasons for an exception to be thrown around here
     */
    public static void main(String... args) throws Exception {
        Toy t;
        switch (args[0]) {
            case "create":
                t = create(args[1]);
                t.write();
                break;
            case "header":
                t = load(args[1]);
                t.printHeader();
                break;
            case "insert":
                t = load(args[1]);
                t.insert();
                t.write();
                break;
            case "display":
                t = load(args[2]);
                t.display(Integer.parseInt(args[1]));
                break;
            case "delete":
                t = load(args[2]);
                t.delete(Integer.parseInt(args[1]));
                t.write();
                break;
            case "search":
                t = load(args[2]);
                for (int i : t.search(args[1])) {
                    System.out.printf("Record %d\n", i);
                    t.display(i);
                }
        }
    }

    /**
     * Attributes that are stored in the database
     */
    private ArrayList<Attribute> atribs = new ArrayList<>();
    /**
     * Records stored in the database
     */
    private ArrayList<HashMap<Attribute, Field>> records = new ArrayList<>();
    /**
     * Database that is being read/written
     */
    private File file;

    /**
     * Creates a new database
     * @param filename
     */
    private Toy(String filename) {
        records = new ArrayList<>();
        atribs = new ArrayList<>();
        file = new File(filename);
    }

    /**
     * Creates a database from scratch
     * @param filename
     * File where the database is to be stored at
     * @return
     * a new database object
     * @throws Exception
     * file stuff?
     */
    private static Toy create(String filename) throws Exception{
        Toy t = new Toy(filename);

        do {
            if (t.atribs.size() > 0) INPUT.nextLine();
            System.out.print("Attribute name: ");
            String name = INPUT.nextLine().trim();
            System.out.print("1. integer\n2. double\n3. boolean\n4. string\nSelect a type: ");
            int type = INPUT.nextInt();
            if (type < 1 || type > 4) {
                System.err.printf("ERROR, INVALID TYPE SELECTION %d\n", type);
                System.exit(-1);
            }
            Attribute a = new Attribute();
            t.atribs.add(a);
            a.name = name;
            a.type = type;
            System.out.print("More attribute? (y/n): ");
        } while ("y".equals(INPUT.next().trim()));
        return t;
    }

    /**
     * Reads a database from disk
     * @param filename
     * filename to read
     * @return
     * the database that was read
     * @throws Exception
     * file stuff?
     */
    private static Toy load(String filename) throws Exception {
        Toy t = new Toy(filename);
        Scanner in = new Scanner(t.file);
        Scanner head = new Scanner(in.nextLine());
        head.useDelimiter(Pattern.compile("[\\[\\]]+"));
        for (int i = 0, attrs = head.nextInt(); i < attrs; i++) {
            String[] attr = head.next().split("[:]");
            Attribute a = new Attribute();
            t.atribs.add(a);
            a.name = attr[0];
            a.type = Integer.parseInt(attr[1]);
        }
        while (in.hasNextLine()) {
            HashMap<Attribute, Field> hm = new HashMap<>();
            t.records.add(hm);
            String line = in.nextLine();
            line = line.substring(1, line.length() - 1);
            String[] tup = line.split("[\\|]");
            for (int i = 0; i < t.atribs.size(); i++) {
                hm.put(t.atribs.get(i), new Field(tup[i], t.atribs.get(i).type));
            }
        }
        in.close();
        head.close();
        return t;
    }

    /**
     * Writes the database to disk
     * @throws Exception
     * file stuff?
     */
    private void write() throws Exception {
        PrintWriter out = new PrintWriter(file);
        out.printf("[%d]", atribs.size());
        for (Attribute a : atribs) {
            out.printf("[%s:%d]", a.name, a.type);
        }
        out.printf("[%d]\n", records.size());

        for (HashMap<Attribute, Field> hm : records) {
            String prefix = "{";
            for (Attribute a : atribs) {
                out.print(prefix);
                prefix = "|";
                out.print(hm.get(a));
            }
            out.println("}");
        }
        out.flush();
        out.close();
    }

    /**
     * Inserts a new record to the database, read from the the specified INPUT
     */
    private void insert() {
        HashMap<Attribute, Field> hm = new HashMap<>();
        records.add(hm);
        for (Attribute a : atribs) {
            System.out.printf("%s: ", a.name);
            String s = INPUT.nextLine();
            hm.put(a, new Field(s, a.type));
        }
    }

    /**
     * Deletes a record from the database
     * @param rid
     * which record to delete
     */
    private void delete(int rid) {
        records.remove(rid);
    }

    /**
     * Prints a record from the database
     * @param rid
     * record to display
     */
    private void display(int rid) {
        for (Attribute a : atribs) {
            System.out.printf("%s: %s\n", a.name, records.get(rid).get(a).toString());
        }
    }

    /**
     * Searches the database by the given query
     * @param query
     * a query in the for "<Attribute> = <Value>"
     * @return
     * the indexes of the foudn values
     */
    private int[] search(String query) {
        String[] sq = query.split("=");
        sq[0] = sq[0].trim();
        Field f = null;
        Attribute comp = null;
        for (Attribute a : atribs) {
            if (a.name.equals(sq[0])) {
                comp = a;
                f = new Field(sq[1].trim(), a.type);
                break;
            }
        }

        ArrayList<Integer> found = new ArrayList<>();
        for (int i = 0; i < records.size(); i++) {
            if (records.get(i).get(comp).equals(f)) found.add(i);
        }

        int[] rtn = new int[found.size()];
        for (int i = 0; i < found.size(); i++) {
            rtn[i] = found.get(i);
        }
        return rtn;
    }

    /**
     * Prints the header of the database
     */
    private void printHeader() {
        System.out.printf("%d attributes\n", atribs.size());
        for (int i = 0; i < atribs.size(); i++) {
            String t = Integer.toString(atribs.get(i).type);
            t = t.replace("1", "integer").replace("2", "double");
            t = t.replace("3", "boolean").replace("4", "String");
            System.out.printf("Attribute %d: %s, %s\n", i + 1, atribs.get(i).name, t);
        }
        System.out.printf("%d records\n", records.size());
    }
    
    /**
     * Class for storing an attribute name and its type
     */
    private static class Attribute {
        /**
         * Attribute name
         */
        String name;
        /**
         * Attribute type
         * 1 = integer
         * 2 = double
         * 3 = boolean
         * 4 = String
         * any other is invalid
         */
        int type;
    }

    /**
     * A field in the database to be associated with an attribute
     */
    private static class Field {
        /**
         * String value
         */
        String string;
        /**
         * integer value
         */
        int integer;
        /**
         * double value
         */
        double dbl;
        /**
         * boolean value
         */
        boolean bool;
        /**
         * type of the field
         */
        int type;

        /**
         * Creates a new field
         * @param in
         * value of this field
         * @param type
         * type of this field
         */
        Field(String in, int type) {
            this.type = type;
            switch (type) {
            case 1:
                integer = Integer.parseInt(in);
                break;
            case 2:
                dbl = Double.parseDouble(in);
                break;
            case 3:
                if ("T".equals(in)) {
                    bool = true;
                } else if ("F".equals(in)) {
                    bool = false;
                } else {
                    throw new RuntimeException(in + " is not a valid boolean, expected either 'T' or 'F'");
                }
                break;
            case 4:
                string = in;
                break;
            default:
                throw new RuntimeException("Invalid type: " + type);
            }
        }

        /**
         * Prints this field as a string
         */
        @Override
        public String toString() {switch (type) {
            case 1:
                return "" + integer;
            case 2:
                return "" + dbl;
            case 3:
            return bool ? "T" : "F";
            case 4:
                return string;
            default:
                return null;
            }
        }

        /**
         * Compares two fields to see if their contents is equal
         */
        @Override
        public boolean equals(Object o) {
            if (o == null || o.getClass() != this.getClass()) return false;
            if (type != ((Field) o).type) return false;
            switch (type) {
            case 1:
                return integer == ((Field) o).integer;
            case 2:
                return dbl == ((Field) o).dbl;
            case 3:
                return bool == ((Field) o).bool;
            case 4:
                if (((Field) o).string == null) return false;
                return string.equals(((Field) o).string);
            default:
                return false;
            }
        }
    }
}