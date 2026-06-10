#include <iostream>
using namespace std;

int main() {

    // ----------------------------
    // 1. Training data
    // ----------------------------
    // x = input values
    // y = correct output values
    float x[][2] = {
        {25, 3 },
        {30,5},
        {50,15},
        {40,8},
        {21, 2 },
        {18, 0}
    };
    float y[] = {100000, 200000, 500000,450000, 40000,20000};
    int n = 6;

    // ----------------------------
    // 2. Model parameters
    // ----------------------------
    // w = weight (controls slope)
    // b = bias (shifts line up/down)
    float w[2] ={0.5,0.5};
    float b = 0.0;

    // learning rate = step size
    float lr = 1e-6;

    // ----------------------------
    // 3. Training loop (epochs)
    // ----------------------------
    for (int epoch = 0; epoch < 10000; epoch++) {
        
        // reset values every epoch
        float loss = 0; // total error
        float dw[2] = {0 ,0};   // gradient for w
        float db = 0;   // gradient for b

        // ----------------------------
        // 4. Loop through data
        // ----------------------------
        for (int i = 0; i < n; i++) {

            // prediction: y = wx + b
            float y_pred = b;

            for(int j = 0; j < 2; j++)
            {
                y_pred += w[j] * x[i][j];
            }
        
            // error = difference between prediction and real value
            float error = y_pred - y[i];

            // squared error (to punish big mistakes)
            loss += error * error;

            // ----------------------------
            // gradients (how wrong direction is)
            // ----------------------------

            // dw = how much w caused error
            // multiply error with input x
            for(int j =0;j<2;j++){
                dw[j] += 2 * error * x[i][j];
            }
            

            // db = how much bias caused error
            // just error itself
            db += 2 * error;
        }

        // ----------------------------
        // 5. Average values
        // ----------------------------
        loss /= n; // average loss
        for(int j=0;j<2;j++){
            dw[j] /=n;
        }
        //dw /= n;   // average gradient for w
        db /= n;   // average gradient for b

        // ----------------------------
        // 6. Update model (learning step)
        // ----------------------------
        // move w and b opposite to error direction
        for(int j=0;j<2;j++){
            w[j]=w[j]-lr*dw[j];
        }
        //w = w - lr * dw;
        b = b - lr * db;

        // print progress every 100 steps
        if (epoch % 100 == 0) {
            cout << "Epoch " << epoch
                 << " Loss: " << loss
                 << " w: " << w
                 << " b: " << b << endl;
        }
    }
    float age, experience;

    cout << "\n--- Salary Predictor ---" << endl;

    while(true) {
        cout << "\nEnter Age (-1 to quit): ";
        cin >> age;
        if(age == -1) break;
    
        cout << "Enter Experience (years): ";
        cin >> experience;
    
        // same formula: y = w[0]*age + w[1]*experience + b
        float predicted_salary = w[0] * age + w[1] * experience + b;
    
        cout << "Predicted Salary: $" << predicted_salary << endl;
    }   
    // ----------------------------
    // 7. Final trained model
    // ----------------------------
    cout << "\nSalary = "
     << w[0] << " * Age + "
     << w[1] << " * Experience + "
     << b << endl;

    return 0;
}