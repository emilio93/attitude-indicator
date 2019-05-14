#include "scheduler/Task.hpp"

scheduler::Task::Task() {
	m_u8TaskID = m_u8NextTaskID;
	m_u8NextTaskID++;
	m_bIsFinished = false;
}
