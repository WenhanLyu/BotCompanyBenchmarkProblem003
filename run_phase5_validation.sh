#!/bin/bash

# Phase 5.2 Final Validation Script
# Tests all 12 cases + detailed timing for 6.in and bigger.in

BINARY="./code"
DATA_DIR="data/003/data_test"
OUTPUT_DIR="phase5_validation_outputs"

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo "========================================="
echo "Phase 5.2 Final Validation"
echo "Date: $(date)"
echo "Binary: $BINARY"
echo "========================================="
echo ""

# Test files in priority order
TEST_FILES=(
  "1.in"
  "error.in"
  "2.in"
  "test.in"
  "small.in"
  "3.in"
  "4.in"
  "5.in"
  "6.in"
  "7.in"
  "big.in"
  "bigger.in"
)

echo "=== Functional Tests (All 12 Cases) ==="
echo ""

for test_file in "${TEST_FILES[@]}"; do
  input_file="$DATA_DIR/$test_file"
  output_file="$OUTPUT_DIR/${test_file%.in}.out"

  if [ ! -f "$input_file" ]; then
    echo "❌ $test_file - FILE NOT FOUND"
    continue
  fi

  echo -n "Testing $test_file ... "

  # Run test with timeout
  if timeout 600 /usr/bin/time -l "$BINARY" < "$input_file" > "$output_file" 2> "$OUTPUT_DIR/${test_file%.in}.time"; then
    # Extract timing info
    time_ms=$(grep "user" "$OUTPUT_DIR/${test_file%.in}.time" | awk '{print $1 * 1000}')
    mem_kb=$(grep "maximum resident set size" "$OUTPUT_DIR/${test_file%.in}.time" | awk '{print $1}')
    mem_mb=$(echo "scale=2; $mem_kb / 1024" | bc)

    echo "✅ PASS (${time_ms}ms, ${mem_mb}MB)"
  else
    echo "❌ FAIL (timeout or error)"
  fi
done

echo ""
echo "=== Detailed Performance Tests ==="
echo ""

# Detailed timing for 6.in
echo "Running 6.in with detailed timing (3 runs)..."
for i in 1 2 3; do
  echo -n "  Run $i: "
  /usr/bin/time -l "$BINARY" < "$DATA_DIR/6.in" > "$OUTPUT_DIR/6_run${i}.out" 2> "$OUTPUT_DIR/6_run${i}.time"
  time_ms=$(grep "user" "$OUTPUT_DIR/6_run${i}.time" | awk '{print $1 * 1000}')
  echo "${time_ms}ms"
done

echo ""

# Detailed timing for bigger.in
echo "Running bigger.in with detailed timing (3 runs)..."
for i in 1 2 3; do
  echo -n "  Run $i: "
  /usr/bin/time -l "$BINARY" < "$DATA_DIR/bigger.in" > "$OUTPUT_DIR/bigger_run${i}.out" 2> "$OUTPUT_DIR/bigger_run${i}.time"
  time_ms=$(grep "user" "$OUTPUT_DIR/bigger_run${i}.time" | awk '{print $1 * 1000}')
  echo "${time_ms}ms"
done

echo ""
echo "========================================="
echo "Validation Complete!"
echo "Results saved to: $OUTPUT_DIR/"
echo "========================================="
