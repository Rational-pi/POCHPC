#include <gtest/gtest.h>
#include <Validation/Validator.h>

// Classe de test
class ValidatorTest : public ::testing::Test {
protected:
    // D�finition des objets communs pour les tests
    void SetUp() override {
        //char* tmp = std::getenv("PROJECT_PATH");
        absoluteDataValidationPath = PROJECT_PATH / relativeDataValidationPath;
    }

    // Nettoyage des ressources allou�es dans SetUp
    void TearDown() override {
        // Code de nettoyage commun
    }
protected:
    std::filesystem::path absoluteDataValidationPath;
    const std::filesystem::path relativeDataValidationPath = "sources/Validation/tests/data";
};

TEST_F(ValidatorTest, NominalCase) {
    // Chemins des fichiers de test
    std::filesystem::path dumpPath = absoluteDataValidationPath / "NominalCase";

    // Le fichier de dump Matlab contient 2 Arrays : 
    // 1. 2. 3.
    // 4. 5. 6.

    // Ajoutez des vecteurs � la liste, par exemple :
    Eigen::ArrayXd array1(3);
    Eigen::ArrayXd array2(3);
    array1 << 1.1, 2.1, 3.1;
    array2 << -4.0, 5.0, 6.0;

    // Comparaison
    Validator::getInstance().compareArrays(dumpPath, std::list<const Eigen::ArrayXd*>{&array1, & array2});

    // Tests du status
    EXPECT_EQ(Validator::getInstance().getStatus(), Validator::Status::Ready);
    
    // Tests sur les donn�es
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    const auto& errors = Validator::getInstance().getErrors();
    // Premier vecteur
    EXPECT_NEAR(errors[0].maxAbsoluteError, 0.1, epsilon);
    EXPECT_NEAR(errors[0].maxRelativeError, 0.1, epsilon);
    EXPECT_NEAR(errors[0].meanAbsoluteError, 0.1, epsilon);
    EXPECT_NEAR(errors[0].meanRelativeError, 0.061111111111111165, epsilon);
    // Deuxi�me vecteur
    EXPECT_NEAR(errors[1].maxAbsoluteError, 8., epsilon);
    EXPECT_NEAR(errors[1].maxRelativeError, 2., epsilon);
    EXPECT_NEAR(errors[1].meanAbsoluteError, 8./3., epsilon);
    EXPECT_NEAR(errors[1].meanRelativeError, 2./3., epsilon);
}

TEST_F(ValidatorTest, AtLeastOneEmptyArray) {
    // Chemins des fichiers de test
    std::filesystem::path dumpPath = absoluteDataValidationPath / "AtLeastOneEmptyArray";

    // Le fichier de dump Matlab contient 2 Arrays : 
    // 1. 2. 3.
    // 4. 5. 6.

    // un vecteur vide : erreur
    Eigen::ArrayXd array1(3);
    Eigen::ArrayXd array2(0);

    Validator::getInstance().compareArrays(dumpPath, std::list<const Eigen::ArrayXd*>{&array1, &array2});

    // Tests du status
    EXPECT_EQ(Validator::getInstance().getStatus(), Validator::Status::Error);

    // Test du message d'erreur
    EXPECT_EQ(Validator::getInstance().getErrorMessage(), "At least one array of the list is empty. Comparison process stopped.");
}

TEST_F(ValidatorTest, InconsistentArraySizes) {
    // Chemins des fichiers de test
    std::filesystem::path dumpPath = absoluteDataValidationPath / "InconsistentArraySizes";

    // Le fichier de dump Matlab contient 2 Arrays : 
    // 1. 2. 3.
    // 4. 5. 6.

    // un vecteur vide : erreur
    Eigen::ArrayXd array1(3);
    Eigen::ArrayXd array2(2);

    Validator::getInstance().compareArrays(dumpPath, std::list<const Eigen::ArrayXd*>{&array1, &array2});

    // Tests du status
    EXPECT_EQ(Validator::getInstance().getStatus(), Validator::Status::Error);

    // Test du message d'erreur
    EXPECT_EQ(Validator::getInstance().getErrorMessage(), "The sizes of the C++ and Matlab arrays are inconsistent. Comparison process stopped.");
}

TEST_F(ValidatorTest, MissingMatlabDump) {
    // Chemins des fichiers de test
    std::filesystem::path dumpPath = absoluteDataValidationPath / "MissingMatlabDump";

    // un vecteur vide : erreur
    Eigen::ArrayXd array1(3);
    Eigen::ArrayXd array2(3);

    Validator::getInstance().compareArrays(dumpPath, std::list<const Eigen::ArrayXd*>{&array1, &array2});

    // Tests du status
    EXPECT_EQ(Validator::getInstance().getStatus(), Validator::Status::Error);

    // Test du message d'erreur
    EXPECT_EQ(Validator::getInstance().getErrorMessage(), "The matlab dump file does not exist.");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}