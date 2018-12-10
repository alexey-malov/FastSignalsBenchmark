#include "libfastsignals/include/signal.h"
#include <benchmark/benchmark.h>
#include <boost/signals2.hpp>
#include <string>

namespace
{
unsigned kBenchEmitCount = 2000;
} // namespace

template <template <class Signature> class Signal>
void emit_signal(benchmark::State& state)
{
	for (auto _ : state)
	{
		volatile int savedValue = 0;
		const auto connCount = state.range();

		Signal<void(int)> valueChanged;
		for (unsigned i = 0; i < connCount; ++i)
		{
			valueChanged.connect([&savedValue](int value) {
				savedValue = value;
			});
		}
		for (unsigned i = 0; i < kBenchEmitCount; ++i)
		{
			valueChanged(i);
		}
	}
}

template <template <class Signature> class Signal>
void emit_signal_with_potentially_throwing_slot(benchmark::State& state)
{
	for (auto _ : state)
	{
		volatile int savedValue = 0;
		const auto connCount = state.range();

		Signal<void(int)> valueChanged;
		for (unsigned i = 0; i < connCount; ++i)
		{
			valueChanged.connect([&savedValue, makes_lambda_potentially_throwing = std::string()](int value) {
				savedValue = value;
			});
		}
		for (unsigned i = 0; i < kBenchEmitCount; ++i)
		{
			valueChanged(i);
		}
	}
}

void emit_fastsignals(benchmark::State& state)
{
	emit_signal<is::signals::signal>(state);
}

void emit_fastsignals_potentially_throwing_slot(benchmark::State& state)
{
	emit_signal_with_potentially_throwing_slot<is::signals::signal>(state);
}

void emit_boost(benchmark::State& state)
{
	emit_signal<boost::signals2::signal>(state);
}

void emit_boost_potentially_throwing_slot(benchmark::State& state)
{
	emit_signal_with_potentially_throwing_slot<boost::signals2::signal>(state);
}

constexpr int64_t NUM_ITERATIONS = 1000;
constexpr int64_t RANGE_MAX = 32;

BENCHMARK(emit_boost)->RangeMultiplier(2)->Range(0, RANGE_MAX)->Iterations(NUM_ITERATIONS);
BENCHMARK(emit_boost_potentially_throwing_slot)->RangeMultiplier(2)->Range(0, RANGE_MAX)->Iterations(NUM_ITERATIONS);
BENCHMARK(emit_fastsignals)->RangeMultiplier(2)->Range(0, RANGE_MAX)->Iterations(NUM_ITERATIONS);
BENCHMARK(emit_fastsignals_potentially_throwing_slot)->RangeMultiplier(2)->Range(0, RANGE_MAX)->Iterations(NUM_ITERATIONS);
