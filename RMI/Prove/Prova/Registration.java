package RMI.Prove.Prova;

public class Registration {
    private String fullName;
    private int courseCode;
    private String id;

    public Registration(String fullName, int courseCode, String id) {
        this.fullName = fullName;
        this.courseCode = courseCode;
        this.id = id;
    }

    public boolean find (String fullName, int courseCode) {
        return this.fullName.equals(fullName) && this.courseCode == courseCode;
    }

    public String getId() {
        return this.id;
    }

    public int getCourseCode() {
        return this.courseCode;
    }
}
