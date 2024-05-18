#pragma once
#include "mt/event.hpp"
#include "mt/execution_context.hpp"
#include "solver/trey.hpp"

namespace teknavo {

struct ParsingContext {
  ParsingContext(int argc, char** argv) : total{argc}, data{argv} {}

  const int total;
  const char* const* const data;
  int last_pos{};
};

class ExecutionManager {
 public:
  ExecutionManager(int argc, char** argv) : parsing_context_{argc, argv} {}

  void Run();
  void Stop();

 private:
  void ProcessTrey();
  void CalculateTrey(teknavo::Trey* trey);
  void PrintTrey(const teknavo::Trey& trey) const;

  void OnParsingComplete(teknavo::Trey::Ptr trey);
  void OnCalculationComplete(teknavo::Trey::Ptr trey);
  void OnProcessingComplete();

  Trey::Ptr ComposeTrey(ParsingContext* context);

 private:
  ExecutionContext parser_;
  ExecutionContext solver_;
  ExecutionContext printer_;
  teknavo::Event<true> done_;
  ParsingContext parsing_context_;
  std::size_t in_progress_task_count_{};
};

}  // namespace teknavo
