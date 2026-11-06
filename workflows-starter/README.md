# Workflows starter (Mistral Workflows)

A minimal, self-contained Mistral Workflows project kept separate from the Expo app.
It registers one workflow, `hello-world`, which takes a `name` and returns a greeting.

## Layout

```
workflows-starter/
  pyproject.toml                        Python project + dependencies (mistralai-workflows >= 3.10)
  apps/worker/src/worker/
    entrypoints/worker.py               Worker entrypoint: discovers workflows, starts the worker
    workflows/hello.py                  The hello-world workflow + its activity
  deploy/docker/Dockerfile.worker       Image used by the managed deployment
  tests/test_hello.py                   Unit test for the activity
```

## Run locally

Requires Python 3.12+, uv, and a Mistral API key.

```bash
cd workflows-starter
uv add "mistralai-workflows[mistralai]>=3.10"
MISTRAL_API_KEY=your-key DEPLOYMENT_NAME=dev-starter uv run python -m worker.entrypoints.worker
```

```bash
MISTRAL_API_KEY=your-key uv run pytest
```

## Trigger an execution

```bash
curl -X POST https://api.mistral.ai/v1/workflows/hello-world/execute \\
  -H "Authorization: Bearer $MISTRAL_API_KEY" \\
  -H "Content-Type: application/json" \\
  -d '{"input": {"name": "Jade"}, "wait_for_result": true}'
```

## Managed deployment

Mistral Cloud clones the repository, builds `deploy/docker/Dockerfile.worker` with
`workflows-starter/` as the build context, and runs the worker. The service-account
token is injected by the platform; no API key lives in the repository.
