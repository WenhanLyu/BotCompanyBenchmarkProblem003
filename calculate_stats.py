import re
import glob

def extract_time_mem(filename):
    with open(filename) as f:
        content = f.read()
    
    # Extract real time (in seconds)
    real_match = re.search(r'(\d+\.\d+)\s+real', content)
    real_time = float(real_match.group(1)) * 1000 if real_match else 0  # Convert to ms
    
    # Extract memory (in KB)
    mem_match = re.search(r'(\d+)\s+maximum resident set size', content)
    mem_kb = int(mem_match.group(1)) if mem_match else 0
    mem_mb = mem_kb / 1024
    
    return real_time, mem_mb

# Analyze 6.in
print("=== 6.in Performance (3 runs) ===")
times_6 = []
for i in range(1, 4):
    time_ms, mem_mb = extract_time_mem(f'phase5_validation_outputs/6_run{i}.time')
    times_6.append(time_ms)
    print(f"Run {i}: {time_ms:.0f}ms, {mem_mb:.2f}MB")

avg_6 = sum(times_6) / len(times_6)
print(f"Average: {avg_6:.0f}ms")
print()

# Analyze bigger.in
print("=== bigger.in Performance (3 runs) ===")
times_bigger = []
for i in range(1, 4):
    time_ms, mem_mb = extract_time_mem(f'phase5_validation_outputs/bigger_run{i}.time')
    times_bigger.append(time_ms)
    print(f"Run {i}: {time_ms:.0f}ms, {mem_mb:.2f}MB")

avg_bigger = sum(times_bigger) / len(times_bigger)
print(f"Average: {avg_bigger:.0f}ms")
print()

# Comparison to Phase 4
print("=== Comparison to Phase 4 Baseline ===")
print()
print("6.in:")
phase4_6 = 730
print(f"  Phase 4: {phase4_6}ms")
print(f"  Phase 5.2: {avg_6:.0f}ms")
improvement_6 = (phase4_6 - avg_6) / phase4_6 * 100
speedup_6 = phase4_6 / avg_6
print(f"  Improvement: {improvement_6:.1f}% faster ({speedup_6:.2f}x speedup)")
print()

print("bigger.in:")
phase4_bigger = 45810
print(f"  Phase 4: {phase4_bigger}ms")
print(f"  Phase 5.2: {avg_bigger:.0f}ms")
improvement_bigger = (phase4_bigger - avg_bigger) / phase4_bigger * 100
speedup_bigger = phase4_bigger / avg_bigger
print(f"  Improvement: {improvement_bigger:.1f}% faster ({speedup_bigger:.2f}x speedup)")
print()

# Target status
print("=== Target Status ===")
print()
target_6 = 1000
target_bigger = 2000

if avg_6 < target_6:
    print(f"✅ 6.in: PASS ({avg_6:.0f}ms < {target_6}ms target)")
else:
    print(f"❌ 6.in: FAIL ({avg_6:.0f}ms > {target_6}ms target)")

if avg_bigger < target_bigger:
    print(f"✅ bigger.in: PASS ({avg_bigger:.0f}ms < {target_bigger}ms target)")
else:
    over_factor = avg_bigger / target_bigger
    print(f"❌ bigger.in: FAIL ({avg_bigger:.0f}ms vs {target_bigger}ms target, {over_factor:.1f}x over)")
