# Temporal NodeJS SDK

![CI](https://github.com/temporalio/sdk-node/actions/workflows/ci.yml/badge.svg)
[![NPM](https://img.shields.io/npm/v/temporalio.svg?style=flat)](https://www.npmjs.com/package/temporalio)

Typescript + NodeJS SDK for [Temporal](temporal.io).

## !!! This is a work in progress, not ready for use yet !!!

For more information see the [proposal](https://github.com/temporalio/proposals/blob/master/node/node-sdk.md).

<!-- vim-markdown-toc GFM -->

- [Getting started](#getting-started)
  - [Install system dependencies](#install-system-dependencies)
  - [Create a new project](#create-a-new-project)
  - [Build everything](#build-everything)
  - [Run the Temporal server](#run-the-temporal-server)
  - [Test your workflow](#test-your-workflow)
- [Hello World](#hello-world)
  - [Activities and workflows](#activities-and-workflows)
  - [Worker and client](#worker-and-client)
- [Development](#development)
  - [Environment set up](#environment-set-up)
  - [Building](#building)
  - [Rebuilding (useful for after deleting Typescript files)](#rebuilding-useful-for-after-deleting-typescript-files)
  - [Building with watch (Typescript only)](#building-with-watch-typescript-only)
  - [Testing](#testing)

<!-- vim-markdown-toc -->

### Getting started

#### Install system dependencies

This project requires nodejs LTS version 12 (or later).

Furthermore, to install this module you will need a c++ compiler and the Rust toolchain installed.
If you run into errors during installation it is likely your environment is not properly set up.

- To set up a C++ compiler for `node-gyp`, follow the instuctions [here](https://github.com/nodejs/node-gyp)
- To set up the Rust toolchain, follow the instruction [here](https://rustup.rs/)

#### Create a new project

```sh
npm init @temporalio ./example
cd ./example
```

> NOTE: `init` triggers native module compilation which will take a while, npm 7 hides the compilation output so it may appear that the installation is stuck, to see the compilation progress export `NPM_CONFIG_FOREGROUND_SCRIPTS=true`. The long wait time will be go away once the SDK [publishes pre-built binaries](https://github.com/temporalio/sdk-node/issues/16).

#### Build everything

```
npm run build
```

#### Run the Temporal server

Download, install, and run the [Temporal server](https://docs.temporal.io/docs/server-quick-install) via docker-compose. It is easy to do and you can keep it running in the background while you build applications.

#### Test your workflow

- Run the worker

  ```sh
  node lib/worker/index.js
  ```

- Run the workflow

  ```sh
  node lib/worker/test.js
  ```

### Hello World

> Not working yet, activities not implemented

#### Activities and workflows

`src/activities/greeter.ts`

```ts
export async function greet(name: string): Promise<string> {
  return `Hello, ${name}!`;
}
```

`src/interfaces/workflows.ts`

```ts
import { Workflow } from '@temporalio/workflow';

export interface Example extends Workflow {
  main(name: string): Promise<string>;
}
```

`src/workflows/example.ts`

```ts
import { Example } from '@interfaces/workflows';
import { greet } from '@activities/greeter';

async function main(name: string): Promise<string> {
  return await greet(name);
}

export const workflow: Example = { main };
```

#### Worker and client

`src/worker/index.ts`

```ts
import { Worker } from '@temporalio/worker';

(async () => {
  // Automatically locate and register activities and workflows
  const worker = new Worker(__dirname);
  // Bind to the `tutorial` queue and start accepting tasks
  await worker.run('tutorial');
})();
```

`src/worker/test.ts`

```ts
import { Connection } from '@temporalio/client';
import { Example } from '@interfaces/workflows';

(async () => {
  const connection = new Connection();
  const example = connection.workflow<Example>('example', { taskQueue: 'tutorial' });
  const result = await example('Temporal');
  console.log(result); // Hello, Temporal
})();
```

### Development

#### Environment set up

```sh
git submodule init
git submodule update
npm ci
```

#### Building

```sh
npm run build
```

#### Rebuilding (useful for after deleting Typescript files)

```sh
npm run rebuild
```

#### Building with watch (Typescript only)

```sh
npm run build  # Must be run once before build.watch
npm run build.watch
```

#### Testing

```sh
npm run test
```

-- OR --

```sh
npm run test.watch
```
