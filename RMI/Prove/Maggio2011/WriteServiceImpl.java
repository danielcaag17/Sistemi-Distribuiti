package RMI.Prove.Maggio2011;

import java.io.FileWriter;
import java.io.IOException;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class WriteServiceImpl extends UnicastRemoteObject implements WriteService {
    public WriteServiceImpl() throws RemoteException {
        super();
    }

    @Override
    public void writeFile(String Path, String s) throws RemoteException {
        try {
            FileWriter myfile = new FileWriter(Path);
            myfile.write(s);
            myfile.close();
        } catch (IOException e) {
            System.out.println(e);
        }
    }    
}
