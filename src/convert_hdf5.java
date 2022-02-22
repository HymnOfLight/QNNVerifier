import org.nd4j.linalg.io.ClassPathResource;


public class convert_hdf5 {
    public void read_hdf5_from_file(String path)
    {
        String fullModel = new ClassPathResource("full_model.h5").getFile().getPath();
        ComputationGraph model = KerasModelImport.importKerasModelAndWeights(fullModel);
    }
}
