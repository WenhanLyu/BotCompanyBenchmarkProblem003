#!/bin/bash

echo "=== Phase 5.2 Timing Analysis ==="
echo ""

# Analyze 6.in runs
echo "6.in Performance (3 runs):"
for i in 1 2 3; do
  time_file="phase5_validation_outputs/6_run${i}.time"
  if [ -f "$time_file" ]; then
    user_time=$(grep "user" "$time_file" | awk '{print $1}')
    mem_kb=$(grep "maximum resident set size" "$time_file" | awk '{print $1}')
    mem_mb=$(echo "scale=2; $mem_kb / 1024" | bc)
    time_ms=$(echo "$user_time * 1000" | bc)
    echo "  Run $i: ${time_ms}ms, ${mem_mb}MB"
  fi
done

# Calculate 6.in average
avg_6=$(grep "user" phase5_validation_outputs/6_run*.time | awk '{sum+=$1; count++} END {print sum/count * 1000}')
echo "  Average: ${avg_6}ms"
echo ""

# Analyze bigger.in runs
echo "bigger.in Performance (3 runs):"
for i in 1 2 3; do
  time_file="phase5_validation_outputs/bigger_run${i}.time"
  if [ -f "$time_file" ]; then
    user_time=$(grep "user" "$time_file" | awk '{print $1}')
    mem_kb=$(grep "maximum resident set size" "$time_file" | awk '{print $1}')
    mem_mb=$(echo "scale=2; $mem_kb / 1024" | bc)
    time_ms=$(echo "$user_time * 1000" | bc)
    echo "  Run $i: ${time_ms}ms, ${mem_mb}MB"
  fi
done

# Calculate bigger.in average
avg_bigger=$(grep "user" phase5_validation_outputs/bigger_run*.time | awk '{sum+=$1; count++} END {print sum/count * 1000}')
echo "  Average: ${avg_bigger}ms"
echo ""

echo "=== Comparison to Phase 4 Baseline ==="
echo ""
echo "6.in:"
echo "  Phase 4: 730ms"
echo "  Phase 5.2: ${avg_6}ms"
improvement_6=$(echo "scale=2; (730 - $avg_6) / 730 * 100" | bc)
speedup_6=$(echo "scale=2; 730 / $avg_6" | bc)
echo "  Improvement: ${improvement_6}% faster (${speedup_6}x speedup)"
echo ""
echo "bigger.in:"
echo "  Phase 4: 45810ms"
echo "  Phase 5.2: ${avg_bigger}ms"
improvement_bigger=$(echo "scale=2; (45810 - $avg_bigger) / 45810 * 100" | bc)
speedup_bigger=$(echo "scale=2; 45810 / $avg_bigger" | bc)
echo "  Improvement: ${improvement_bigger}% faster (${speedup_bigger}x speedup)"
echo ""

# Check targets
echo "=== Target Status ==="
echo ""
if (( $(echo "$avg_6 < 1000" | bc -l) )); then
  echo "✅ 6.in: PASS (<1000ms target)"
else
  echo "❌ 6.in: FAIL (${avg_6}ms vs 1000ms target)"
fi

if (( $(echo "$avg_bigger < 2000" | bc -l) )); then
  echo "✅ bigger.in: PASS (<2000ms target)"
else
  echo "❌ bigger.in: FAIL (${avg_bigger}ms vs 2000ms target)"
fi

