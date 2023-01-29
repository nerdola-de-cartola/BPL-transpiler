import { expect, test } from "vitest";
import { readFile } from "node:fs/promises";
import { execa } from "execa";

const tests = [
  {
    input: "./array/t1.bpl",
    output: "./array/t1.s",
  },
  {
    input: "./array/t2.bpl",
    output: "./array/t2.s",
  },
  {
    input: "./assignment/t1.bpl",
    output: "./assignment/t1.s",
  },
  {
    input: "./conditions/t1.bpl",
    output: "./conditions/t1.s",
  },
  {
    input: "./functions/t1.bpl",
    output: "./functions/t1.s",
  },
  {
    input: "./functions/t2.bpl",
    output: "./functions/t2.s",
  },
  {
    input: "./variables/t1.bpl",
    output: "./variables/t1.s",
  },
  {
    input: "./variables/t2.bpl",
    output: "./variables/t2.s",
  },
];

test("it generates the expected assembly output", () => {
  tests.forEach(async (test) => {
    // compilation test
    console.log(`Testing ${test.input}...`);
    const input = await readFile(test.input, "utf-8");
    const expectedOutput = await readFile(test.output, "utf-8");
    const { exitCode: compilationExitCode } = await execa("compiler", [
      input,
      `${test.output}.tmp`,
    ]);
    const output = await readFile(`${test.output}.tmp`, "utf-8");
    expect(output).toBe(expectedOutput);
    expect(compilationExitCode).toBe(0);

    // execution test
    const { exitCode: gccExitCode } = await execa("gcc", [
      "-o",
      `${test.output}.exe`,
      `${test.output}.tmp`,
    ]);
    expect(gccExitCode).toBe(0);
    const { exitCode: executionExitCode } = await execa(`${test.output}.exe`);
    expect(executionExitCode).toBe(0);

    // cleanup
    await execa("rm", [`${test.output}.tmp`]);
    await execa("rm", [`${test.output}.exe`]);
  });
});
