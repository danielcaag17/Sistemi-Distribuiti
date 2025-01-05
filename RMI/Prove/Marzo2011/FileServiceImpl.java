package RMI.Prove.Marzo2011;

import java.io.FileWriter;
import java.io.IOException;
import java.rmi.*;
import java.rmi.server.*;

public class FileServiceImpl extends UnicastRemoteObject implements FileService {
    private final String filePath;

    public FileServiceImpl(String filePath) throws RemoteException {
        this.filePath = filePath;
    }

    @Override
    public void writeToFile(String s) throws RemoteException {
        try (FileWriter writer = new FileWriter(filePath, true)) {
            writer.write(s + "\n");
            System.out.println("Escrito: " + s);
        } catch (IOException e) {
            throw new RemoteException("Error escribiendo en el archivo", e);
        }
    }
}