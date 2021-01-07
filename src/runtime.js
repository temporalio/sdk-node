globalThis.print = (...args) => void $0.applyIgnored(undefined, args, { arguments: { copy: true } });
globalThis.sleep = async (ms) => new Promise((resolve) => runtime.timeout(resolve, ms));
globalThis.runActivity = async (name, args, options) =>
  new Promise((resolve, reject) => runtime.runActivity(name, args, options, (err, value) => {
    if (err) reject(err);
    else resolve(value);
  }));
