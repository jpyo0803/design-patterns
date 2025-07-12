#include <iostream>
#include <string>

/*
  Known ingredients for CokaCola
  - CarbonatedWater
  - SugerSyrup
  - Caramel
  - PhosphoricAcid
  - NaturalFlaver
  - Caffeine

  Unknown ingredient for CokaCola
  - SecretIngredient
*/

/*
  Let's say CokaCola company wants clients to be able to copy their product
  CokaCola, but they don't want to expose the secret ingredient.

  We can use Prototype pattern to achieve this.
  The Prototype pattern allows us to create a new object by copying an existing object.
  In this case, we will copy the known ingredients of CokaCola, but not the unknown secret
  ingredient. This way, we can create a new CokaCola object without exposing the secret ingredient.
  The copied object will have the same known ingredients, but the secret ingredient
  will remain unknown in the copied object.
*/

class Prototype {
 public:
  virtual Prototype* Clone() const = 0;
  virtual ~Prototype() = default;
};

class CokaCola : public Prototype {
 public:
  CokaCola() = default;

  CokaCola(const CokaCola& other) {
    carbonated_water_ = other.carbonated_water_;
    suger_syrup_ = other.suger_syrup_;
    caramel_ = other.caramel_;
    phosphoric_acid_ = other.phosphoric_acid_;
    natural_flaver_ = other.natural_flaver_;
    caffeine_ = other.caffeine_;

    /*
      User can copy the secret ingredient without exposing it.
    */
    secret_ingredient_ = other.secret_ingredient_;
  }

  CokaCola* Clone() const override {
    return new CokaCola(*this);
  }

  void PrintIngredients() const {
    std::cout << "CokaCola Ingredients:" << std::endl;
    std::cout << "Carbonated Water: " << (carbonated_water_.empty() ? "None" : carbonated_water_)
              << std::endl;
    std::cout << "Suger Syrup: " << (suger_syrup_.empty() ? "None" : suger_syrup_) << std::endl;
    std::cout << "Caramel: " << (caramel_.empty() ? "None" : caramel_) << std::endl;
    std::cout << "Phosphoric Acid: " << (phosphoric_acid_.empty() ? "None" : phosphoric_acid_)
              << std::endl;
    std::cout << "Natural Flaver: " << (natural_flaver_.empty() ? "None" : natural_flaver_)
              << std::endl;
    std::cout << "Caffeine: " << (caffeine_.empty() ? "None" : caffeine_) << std::endl;
    std::cout << "Secret Ingredient: "
              << (secret_ingredient_.empty() ? "Unknown" : secret_ingredient_) << std::endl;
    std::cout << "-----------------------" << std::endl;
  }

  void set_secret_ingredient(const std::string& ingredient) {
    secret_ingredient_ = ingredient;
  }

 public:
  // Known ingredients
  std::string carbonated_water_;
  std::string suger_syrup_;
  std::string caramel_;
  std::string phosphoric_acid_;
  std::string natural_flaver_;
  std::string caffeine_;

 private:
  // Unknown ingredient
  std::string secret_ingredient_;  // Unknown ingredient
};

int main() {
  /*
    CokaCola is a product with known and unknown ingredients.
    The known ingredients are CarbonatedWater, SugerSyrup, Caramel,
    PhosphoricAcid, NaturalFlaver, and Caffeine.
    The unknown ingredient is SecretIngredient.
  */
  CokaCola* coka_cola = new CokaCola();
  coka_cola->carbonated_water_ = "Carbonated Water";
  coka_cola->suger_syrup_ = "Suger Syrup";
  coka_cola->caramel_ = "Caramel";
  coka_cola->phosphoric_acid_ = "Phosphoric Acid";
  coka_cola->natural_flaver_ = "Natural Flaver";
  coka_cola->caffeine_ = "Caffeine";
  coka_cola->set_secret_ingredient("Secret Ingredient");
  coka_cola->PrintIngredients();

  CokaCola* copied_coka_cola = coka_cola->Clone();

  copied_coka_cola->PrintIngredients();

  delete coka_cola;
  delete copied_coka_cola;

  return 0;
}