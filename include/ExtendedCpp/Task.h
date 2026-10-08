#ifndef Common_Task_H
#define Common_Task_H

#include <coroutine>
#include <mutex>
#include <exception>
#include <future>
#include <utility>

/// @brief
namespace ExtendedCpp
{
	/// @brief
	/// @tparam TResult
	template<typename TResult>
	class Task final
	{
	public:
		/// @brief
		struct Promise;

		/// @brief
		using Handle = std::coroutine_handle<Promise>;

		/// @brief
		struct FinalAwaiter final
		{
			/// @brief
			FinalAwaiter() noexcept = default;

			/// @brief
			~FinalAwaiter() noexcept = default;

			/// @brief
			/// @return
			static bool await_ready() noexcept { return false; }

			/// @brief
			/// @tparam TPromise
			/// @param handle
			/// @return
			template<typename TPromise>
			std::coroutine_handle<> await_suspend(std::coroutine_handle<TPromise> handle) noexcept
			{
				TPromise& promise = handle.promise();

				std::coroutine_handle<> continuation;

				{
					std::lock_guard lock(promise._mutex);

					promise._done = true;
					continuation = promise._continuation;
					promise._continuation = {};
				}

				promise._condition.notify_all();

				if (continuation)
					return continuation;

				return std::noop_coroutine();
			}

			/// @brief
			static void await_resume() noexcept {}
		};

		/// @brief
		struct ContinueAwaiter final
		{
			/// @brief
			/// @param handle
			explicit ContinueAwaiter(const Handle handle) noexcept : _handle(handle) {}

			/// @brief
			~ContinueAwaiter() noexcept = default;

			/// @brief
			/// @return
			[[nodiscard]]
			bool await_ready() const noexcept
			{
				if (!_handle)
					return true;

				const auto& promise = _handle.promise();
				std::lock_guard lock(promise._mutex);
				return promise._done;
			}

			/// @brief
			/// @param continuation
			/// @return
			std::coroutine_handle<> await_suspend(std::coroutine_handle<> continuation) noexcept
			{
				auto& promise = _handle.promise();

				std::lock_guard lock(promise._mutex);

				if (promise._done)
					return continuation;

				promise._continuation = continuation;

				return std::noop_coroutine();
			}

			/// @brief
			/// @return
			TResult await_resume()
			{
				const auto& promise = _handle.promise();

				std::lock_guard lock(promise._mutex);

				if (promise._exception)
					std::rethrow_exception(promise._exception);

				return std::move(promise._result);
			}

		private:
			Handle _handle;
		};

		/// @brief
		/// @return
		ContinueAwaiter operator co_await() const noexcept
		{
			return ContinueAwaiter{ _handle };
		}

		/// @brief
		/// @tparam T
		template<typename T>
		class FutureAwaiter
		{
		public:
			/// @brief
			/// @param future
			explicit FutureAwaiter(std::future<T>&& future) noexcept
				: _future(std::make_shared<std::future<T>>(std::move(future))) {}

			/// @brief
			~FutureAwaiter() noexcept = default;

			/// @brief
			/// @return
			static bool await_ready() noexcept { return false; }

			/// @brief
			/// @param handle
			void await_suspend(std::coroutine_handle<> handle) const noexcept
			{
				auto future = _future;
				std::thread([future, handle]
				{
					future->wait();
					handle.resume();
				}).detach();
			}

			/// @brief
			/// @return
			T await_resume() const noexcept
			{
				return _future->get();
			}

		private:
			std::shared_ptr<std::future<T>> _future;
		};

		/// @brief
		struct Promise final
		{
			/// @brief
			Promise() noexcept = default;

			/// @brief
			~Promise() noexcept = default;

			/// @brief
			/// @return
			Task get_return_object() noexcept { return Task{ Handle::from_promise(*this) }; }

			/// @brief
			/// @return
			static std::suspend_never initial_suspend() noexcept { return {}; }

			/// @brief
			/// @return
			static FinalAwaiter final_suspend() noexcept { return {}; }

			/// @brief
			/// @param result
			void return_value(const TResult& result) noexcept
			{
				std::lock_guard lock(_mutex);
				_result = result;
			}

			///
			/// @param result
			void return_value(TResult&& result) noexcept
			{
				std::lock_guard lock(_mutex);
				_result = std::move(result);
			}

			/// @brief
			void unhandled_exception() noexcept
			{
				std::lock_guard lock(_mutex);
				_exception = std::current_exception();
			}

		private:
			friend class Task;

			mutable std::mutex _mutex;
			std::condition_variable _condition;

			std::coroutine_handle<> _continuation;
			bool _done = false;
			TResult _result;
			std::exception_ptr _exception;
		};

		/// @brief
		using promise_type = Promise;

		/// @brief
		/// @param handle
		explicit Task(const Handle handle) noexcept : _handle(handle) {}

		/// @brief
		Task(const Task&) noexcept = delete;

		/// @brief
		/// @return
		Task& operator=(const Task&) noexcept = delete;

		/// @brief
		/// @param other
		Task(Task&& other) noexcept : _handle(std::exchange(other._handle, {})) {}

		/// @brief
		/// @param other
		/// @return
		Task& operator=(Task&& other) noexcept
		{
			if (this == &other)
				return *this;

			DestroyHandle();

			_handle = std::exchange(other._handle, {});
			return *this;
		}

		/// @brief
		~Task() noexcept
		{
			DestroyHandle();
		}

		/// @brief
		/// @return
		[[nodiscard]]
		bool IsDone() const noexcept
		{
			if (!_handle)
				return false;

			const auto& promise = _handle.promise();
			std::lock_guard lock(promise._mutex);
			return promise._done;
		}

		/// @brief
		void Wait() const noexcept
		{
			if (!_handle)
				return;

			auto& promise = _handle.promise();
			std::unique_lock lock(promise._mutex);
			promise._condition.wait(lock, [&promise]{ return promise._done; });
		}

		/// @brief
		/// @return
		TResult Result() const
		{
			Wait();

			const auto& promise = _handle.promise();

			std::lock_guard lock(promise._mutex);

			if (promise._exception)
				std::rethrow_exception(promise._exception);

			return std::move(promise._result);
		}

		/// @brief
		/// @tparam TOperation
		/// @tparam Args
		/// @param operation
		/// @param args
		/// @return
		template<typename TOperation, typename... Args>
		static Task Run(TOperation&& operation, Args&&... args)
		{
			std::future<TResult> future =
				std::async(std::launch::async, std::forward<TOperation>(operation), std::forward<Args>(args)...);
			co_return co_await FutureAwaiter<TResult>(std::move(future));
		}

	private:
		void DestroyHandle() noexcept
		{
			if (!_handle)
				return;

			{
				auto& promise = _handle.promise();
				std::unique_lock lock(promise._mutex);
				promise._condition.wait(lock, [&promise]{ return promise._done; });
			}

			_handle.destroy();
			_handle = {};
		}

		Handle _handle;
		static_assert(std::movable<TResult>);
	};

	/// @brief
	template<>
	class Task<void> final
	{
	public:
		/// @brief
		struct Promise;

		/// @brief
		using Handle = std::coroutine_handle<Promise>;

		/// @brief
		struct FinalAwaiter final
		{
			/// @brief
			/// @return
			static bool await_ready() noexcept { return false; }

			/// @brief
			/// @tparam TPromise
			/// @param handle
			/// @return
			template<typename TPromise>
			std::coroutine_handle<> await_suspend(std::coroutine_handle<TPromise> handle) noexcept
			{
				TPromise& promise = handle.promise();

				std::coroutine_handle<> continuation;

				{
					std::lock_guard lock(promise._mutex);

					promise._done = true;
					continuation = promise._continuation;
					promise._continuation = {};
				}

				promise._condition.notify_all();

				if (continuation)
					return continuation;

				return std::noop_coroutine();
			}

			/// @brief
			static void await_resume() noexcept {}
		};

		/// @brief
		struct ContinueAwaiter final
		{
			/// @brief
			/// @param handle
			explicit ContinueAwaiter(const Handle handle) noexcept : _handle(handle) {}

			/// @brief
			/// @return
			[[nodiscard]]
			bool await_ready() const noexcept
			{
				if (!_handle)
					return true;

				const auto& promise = _handle.promise();
				std::lock_guard lock(promise._mutex);
				return promise._done;
			}

			/// @brief
			/// @param continuation
			/// @return
			[[nodiscard]]
			std::coroutine_handle<> await_suspend(const std::coroutine_handle<> continuation) const noexcept
			{
				auto& promise = _handle.promise();

				std::lock_guard lock(promise._mutex);

				if (promise._done)
					return continuation;

				promise._continuation = continuation;

				return std::noop_coroutine();
			}

			/// @brief
			void await_resume() const
			{
				const auto& promise = _handle.promise();

				std::lock_guard lock(promise._mutex);

				if (promise._exception)
					std::rethrow_exception(promise._exception);
			}

		private:
			Handle _handle;
		};

		/// @brief
		/// @return
		ContinueAwaiter operator co_await() const noexcept
		{
			return ContinueAwaiter{ _handle };
		}

		/// @brief
		class FutureAwaiter
		{
		public:
			/// @brief
			/// @param future
			explicit FutureAwaiter(std::future<void>&& future) noexcept
				: _future(std::make_shared<std::future<void>>(std::move(future))) {}

			/// @brief
			/// @return
			static bool await_ready() noexcept { return false; }

			/// @brief
			/// @param handle
			void await_suspend(std::coroutine_handle<> handle) const noexcept
			{
				auto future = _future;
				std::thread([future, handle]
				{
					future->wait();
					handle.resume();
				}).detach();
			}

			/// @brief
			static void await_resume() noexcept {}

		private:
			std::shared_ptr<std::future<void>> _future;
		};

		/// @brief
		struct Promise final
		{
			/// @brief
			Promise() noexcept = default;

			/// @brief
			~Promise() noexcept = default;

			/// @brief
			/// @return
			Task get_return_object() noexcept { return Task{ Handle::from_promise(*this) }; }

			/// @brief
			/// @return
			static std::suspend_never initial_suspend() noexcept { return {}; }

			/// @brief
			/// @return
			static FinalAwaiter final_suspend() noexcept { return {}; }

			/// @brief
			static void return_void() noexcept {}

			/// @brief
			void unhandled_exception() noexcept
			{
				std::lock_guard lock(_mutex);
				_exception = std::current_exception();
			}

		private:
			friend class Task;

			mutable std::mutex _mutex;
			std::condition_variable _condition;

			std::coroutine_handle<> _continuation;
			bool _done = false;
			std::exception_ptr _exception;
		};

		/// @brief
		using promise_type = Promise;

		/// @brief
		/// @param handle
		explicit Task(const Handle handle) noexcept : _handle(handle) {}

		/// @brief
		Task(const Task&) noexcept = delete;

		/// @brief
		/// @return
		Task& operator=(const Task&) noexcept = delete;

		/// @brief
		/// @param other
		Task(Task&& other) noexcept : _handle(std::exchange(other._handle, {})) {}

		/// @brief
		/// @param other
		/// @return
		Task& operator=(Task&& other) noexcept
		{
			if (this == &other)
				return *this;

			DestroyHandle();

			_handle = std::exchange(other._handle, {});
			return *this;
		}

		/// @brief
		~Task() noexcept
		{
			DestroyHandle();
		}

		/// @brief
		/// @return
		[[nodiscard]]
		bool IsDone() const noexcept
		{
			if (!_handle)
				return false;

			const auto& promise = _handle.promise();
			std::lock_guard lock(promise._mutex);
			return promise._done;
		}

		/// @brief
		void Wait() const noexcept
		{
			if (!_handle)
				return;

			auto& promise = _handle.promise();
			std::unique_lock lock(promise._mutex);
			promise._condition.wait(lock, [&promise]{ return promise._done; });
		}

		/// @brief
		/// @tparam TOperation
		/// @tparam Args
		/// @param operation
		/// @param args
		/// @return
		template<typename TOperation, typename... Args>
		static Task Run(TOperation&& operation, Args&&... args)
		{
			std::future<void> future =
				std::async(std::launch::async, std::forward<TOperation>(operation), std::forward<Args>(args)...);
			co_return co_await FutureAwaiter(std::move(future));
		}

	private:
		void DestroyHandle() noexcept
		{
			if (!_handle)
				return;

			{
				auto& promise = _handle.promise();
				std::unique_lock lock(promise._mutex);
				promise._condition.wait(lock, [&promise]{ return promise._done; });
			}

			_handle.destroy();
			_handle = {};
		}

		Handle _handle;
	};
}

#endif