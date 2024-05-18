#include "solver/execution_manager.hpp"

#include <array>
#include <charconv>
#include <cstdint>
#include <iostream>

namespace teknavo {

static constexpr int kMaxProcessingTreys = 1'024;

Trey::Ptr ExecutionManager::ComposeTrey(ParsingContext* context) {
  static constexpr int kRequiredValues = 3;

  int parsed_count = 0;
  std::array<std::int64_t, kRequiredValues> parsed_values;

  while (++context->last_pos < context->total) {
    std::int64_t value;
    const auto pos = context->last_pos;
    const auto* const raw_str = context->data[pos];
    const auto str = std::string_view{raw_str};
    const auto [ptr, ec] =
        std::from_chars(str.data(), str.data() + str.size(), value);
    if (ec != std::errc() || raw_str + str.size() > ptr) {
      printer_.Post([str, pos] {
        std::cout << "Invalid value " << str << " at positition " << pos
                  << "; Ignore..." << std::endl;
      });
      continue;
    }

    parsed_values[parsed_count++] = value;
    if (parsed_count >= kRequiredValues) {
      break;
    }
  }

  assert(parsed_count <= kRequiredValues);

  if (parsed_count != kRequiredValues) {
    return nullptr;
  }

  return std::make_unique<teknavo::Trey>(parsed_values[0], parsed_values[1],
                                         parsed_values[2]);
}

void ExecutionManager::Run() {
  parser_.Run();
  solver_.Run();
  printer_.Run();

  parser_.Post([this] { ProcessTrey(); });
}

void ExecutionManager::Stop() {
  done_.Wait();

  parser_.Stop();
  solver_.Stop();
  printer_.Stop();
}

void ExecutionManager::ProcessTrey() {
  bool done = true;
  while (auto trey = ComposeTrey(&parsing_context_)) {
    OnParsingComplete(std::move(trey));
    if (++in_progress_task_count_ > kMaxProcessingTreys) {
      done = false;
      break;
    }
  }

  if (done) {
    done_.Signal();
  }
}

void ExecutionManager::CalculateTrey(teknavo::Trey* trey) {
  trey->CalculateRoots();
  trey->CalculateExtreme();
}

void ExecutionManager::PrintTrey(const teknavo::Trey& trey) const {
  std::cout << trey << std::endl;
}

void ExecutionManager::OnParsingComplete(teknavo::Trey::Ptr trey) {
  solver_.Post([this, trey = std::move(trey)]() mutable {
    CalculateTrey(trey.get());

    OnCalculationComplete(std::move(trey));
  });
}

void ExecutionManager::OnCalculationComplete(teknavo::Trey::Ptr trey) {
  printer_.Post([this, trey = std::move(trey)] {
    PrintTrey(*trey);

    OnProcessingComplete();
  });
}

void ExecutionManager::OnProcessingComplete() {
  parser_.Post([this] {
    --in_progress_task_count_;

    ProcessTrey();
  });
}

}  // namespace teknavo
