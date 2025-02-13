#include "CelestialObject.h"
#include <iostream> // Hata ayıklama çıktıları için

// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    
    
    this->right_rotation=this;
    this->left_rotation=this;
    
}   
void CelestialObject::handleRotations() {
    
    int rows = shape.size();
    int cols = shape[0].size();

    // Create r1, r2, r3 for rotated shapes
    vector<vector<bool>> r1(cols, vector<bool>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            r1[j][rows - i - 1] = shape[i][j];
        }
    }

    int r1_rows = r1.size();
    int r1_cols = r1[0].size();

    // Rotate r1 to create r2
    vector<vector<bool>> r2(r1_cols, vector<bool>(r1_rows));
    for (int i = 0; i < r1_rows; ++i) {
        for (int j = 0; j < r1_cols; ++j) {
            r2[j][r1_rows - i - 1] = r1[i][j];
        }
    }

    int r2_rows = r2.size();
    int r2_cols = r2[0].size();

    // Rotate r2 to create r3
    vector<vector<bool>> r3(r2_cols, vector<bool>(r2_rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            r3[j][rows - i - 1] = r2[i][j];
        }
    }

    // Establish rotation links only if there are unique shapes
    if (r1 == shape && r3 == shape) {
        // No need to link to new objects, as it's a repeating shape
        this->right_rotation = this;
        this->left_rotation = this;
    } else if (r1 == r3 && shape != r1) {
        this->right_rotation = new CelestialObject(r1, this->object_type, this->starting_row, this->time_of_appearance);
        this->right_rotation->right_rotation = this;
        this->right_rotation->left_rotation = this;
        this->left_rotation = this->right_rotation;
        this->right_rotation->next_celestial_object=this->next_celestial_object;
        

    } else {
        this->right_rotation = new CelestialObject(r1, this->object_type, this->starting_row, this->time_of_appearance);
        this->right_rotation->right_rotation = new CelestialObject(r2, this->object_type, this->starting_row, this->time_of_appearance);
        this->right_rotation->right_rotation->right_rotation = new CelestialObject(r3, this->object_type, this->starting_row, this->time_of_appearance);

        // Link back to the original to complete the cycle
        this->right_rotation->right_rotation->right_rotation->right_rotation = this;

        // Set left_rotation pointers for reverse traversal
        this->left_rotation = this->right_rotation->right_rotation->right_rotation;
        this->right_rotation->left_rotation = this;
        this->right_rotation->right_rotation->left_rotation = this->right_rotation;
        this->right_rotation->right_rotation->right_rotation->left_rotation = this->right_rotation->right_rotation;

        this->right_rotation->next_celestial_object=this->next_celestial_object;
        this->right_rotation->right_rotation->next_celestial_object=this->next_celestial_object;
        this->right_rotation->right_rotation->right_rotation->next_celestial_object=this->next_celestial_object;

        

    }
    
}



// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape), 
          object_type(other->object_type), 
          starting_row(other->starting_row), 
          time_of_appearance(other->time_of_appearance) {
    
    // Initialize rotations as nullptr initially to avoid recursion issues
    this->right_rotation = nullptr;
    this->left_rotation = nullptr;

    // Deep copy of next_celestial_object if it exists
    this->next_celestial_object = other->next_celestial_object;
}



// Function to delete rotations of a given celestial object.
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (target == nullptr || target->right_rotation == target) {
        return;  // No dynamically allocated rotations to delete
    }

    CelestialObject *p = target->right_rotation;
    target->right_rotation = target; // Break the loop

    while (p != target) {
        CelestialObject *q = p;
        p=p->right_rotation;
        delete q;

    }
}
