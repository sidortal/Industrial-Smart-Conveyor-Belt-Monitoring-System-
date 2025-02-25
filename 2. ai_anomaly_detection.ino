#include "model_data.h"  // Pre-trained AI model
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

const int input_size = 2;  // Vibration & Weight Data
float input_data[input_size] = {1.2, 50.0}; // Sample input

void setup() {
  Serial.begin(115200);
  Serial.println("Running AI anomaly detection...");
}

void loop() {
  float anomaly_score = run_ai_model(input_data);
  Serial.print("Anomaly Score: "); Serial.println(anomaly_score);

  if (anomaly_score > 0.8) {
    Serial.println("⚠️ Alert! Possible Conveyor Fault Detected.");
  }

  delay(5000);
}
