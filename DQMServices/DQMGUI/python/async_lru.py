import asyncio
from collections import OrderedDict
from functools import _CacheInfo, _make_key, partial, wraps

__all__ = ('alru_cache',)


def unpartial(fn):
    while hasattr(fn, 'func'):
        fn = fn.func
    return fn

def _cache_invalidate(wrapped, typed, *args, **kwargs):
    key = _make_key(args, kwargs, typed)

    exists = key in wrapped._cache

    if exists:
        wrapped._cache.pop(key)

    return exists


def _cache_clear(wrapped):
    wrapped.hits = wrapped.misses = 0
    wrapped._cache = OrderedDict()



def _cache_info(wrapped, maxsize):
    return _CacheInfo(
        wrapped.hits,
        wrapped.misses,
        maxsize,
        len(wrapped._cache),
    )


def __cache_touch(wrapped, key):
    try:
        wrapped._cache.move_to_end(key)
    except KeyError:  # not sure is it possible
        pass


def _cache_hit(wrapped, key):
    wrapped.hits += 1
    __cache_touch(wrapped, key)


def _cache_miss(wrapped, key):
    wrapped.misses += 1
    __cache_touch(wrapped, key)


def alru_cache(
    fn=None,
    maxsize=128,
    typed=False,
    *,
    cache_exceptions=True,
):
    def wrapper(fn):
        _origin = unpartial(fn)

        if not asyncio.iscoroutinefunction(_origin):
            raise RuntimeError(
                'Coroutine function is required, got {}'.format(fn))

        # functools.partialmethod support
        if hasattr(fn, '_make_unbound_method'):
            fn = fn._make_unbound_method()

        @wraps(fn)
        async def wrapped(*fn_args, **fn_kwargs):
            key = _make_key(fn_args, fn_kwargs, typed)

            # We store (mutable) lists in the cache, so that even if an item
            # is evicted before its task finished, other waiting tasks can get
            # the result. We need to keep a reference to the list here, rather
            # than unpacking.
            event_result_exc = wrapped._cache.get(key)

            if event_result_exc is None:
                event_result_exc = [asyncio.Event(), None, None]
                # logically there is a possible race between get above and 
                # insert here. Make sure there is no `await` in between.
                wrapped._cache[key] = event_result_exc
                _cache_miss(wrapped, key)

                if maxsize is not None and len(wrapped._cache) > maxsize:
                    wrapped._cache.popitem(last=False)

                try:
                    res = await fn(*fn_args, **fn_kwargs)
                    event_result_exc[1] = res
                    return res
                except Exception as e:
                    event_result_exc[2] = e
                    # Even with cache_exceptions=False, we don't retry for
                    # requests that happened *in parallel* to the first one.
                    # Only once the initial taks has failed, the next request
                    # will not hit the cache.
                    if cache_exceptions == False:
                        _cache_invalidate(wrapped, typed, *fn_args, **fn_kwargs)
                    # make sure to pass on the exception to get a proper traceback.
                    raise
                finally:
                    # now at least result or exc is set, and we can release others.
                    event_result_exc[0].set()
            else:
                _cache_hit(wrapped, key)
                # this will return immediatly if the task is done, no need to
                # check manually.
                await event_result_exc[0].wait()
                if event_result_exc[2]:
                    raise event_result_exc[2]
                else:
                    return event_result_exc[1]

        _cache_clear(wrapped)
        wrapped._origin = _origin
        wrapped.cache_info = partial(_cache_info, wrapped, maxsize)
        wrapped.cache_clear = partial(_cache_clear, wrapped)
        wrapped.invalidate = partial(_cache_invalidate, wrapped, typed)

        return wrapped

    if fn is None:
        return wrapper

    if callable(fn) or hasattr(fn, '_make_unbound_method'):
        return wrapper(fn)

    raise NotImplementedError('{} decorating is not supported'.format(fn))
