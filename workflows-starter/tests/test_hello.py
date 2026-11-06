import asyncio

from worker.workflows.hello import greet


def test_greet_returns_greeting() -> None:
    assert asyncio.run(greet("Ada")) == "Hello, Ada! Welcome to Mistral Workflows."
