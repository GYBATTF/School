import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class ConvertCreate {
    public static void main(String... args) throws Exception {
        File f = new File("CreateTables.sql");
        Scanner sc = new Scanner(f);
        ArrayList<String> query = new ArrayList<>();

        boolean comment = false;
        while (sc.hasNextLine()) {
            String line = sc.nextLine().trim();

            if (line.contains("/*")) comment = true;
            if (line.contains("*/")) {comment = false; continue;}
            if (comment) continue;

            if (query.size() > 0 && "".equals(line)) {
                printQuery(query);
            } else {
                query.add(line);
            }
        }
        printQuery(query);


        sc.close();
    }

    private static void printQuery(ArrayList<String> query) {
        for (String s : query) {
            if (s.contains("CREATE TABLE")) {
                String[] q = s.split(" ");
                int table = new ArrayList<String>(Arrays.asList(q)).indexOf("TABLE") + 1;
                System.out.print("private static String " + q[table].toUpperCase() + " = \"");
            }

            System.out.print(s);
        }
        System.out.println("\b\";");
        query.clear();
    }
}