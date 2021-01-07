import ivm from 'isolated-vm';
import path from 'path';
import { readFile } from 'fs/promises';
import '../build/Release/temporalio-workflow-runtime.node';

async function run() {
  const isolate = new ivm.Isolate();
  const context = await isolate.createContext();

  const runtime = new ivm.NativeModule(require.resolve('../build/Release/temporalio-workflow-runtime.node'));
  const instance = await runtime.create(context);
  await context.global.set('runtime', instance.derefInto());

  const script = await readFile(path.resolve(__dirname, '../src/runtime.js'), 'utf8');
  await context.evalClosure(script, [console.log], { arguments: { reference: true } });

  await new Promise(async (resolve, reject) => {
    try {
      await context.evalClosure(`
        (async () => {
          sleep(1000).then(() => print(1000));
          print('await');
          await sleep(100);
          print(100);
          print('random', runtime.random());
          print('date', new Date(runtime.date()));
        })().then(() => $0.apply(undefined, []), (err) => $1.apply(undefined, [err]));
      `, [resolve, reject], { arguments: { reference: true }, result: { copy: true } });
    } catch (err) {
      reject(err);
    }
  });

  console.log('done');
}

run().catch((err) => {
  console.error('Error:', err);
  process.exit(1);
});
