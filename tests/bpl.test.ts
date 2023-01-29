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
    input: "./variables/t1.bpl",
    output: "./variables/t1.s",
  },
];

test("it generates the expected assembly output", () => {
  tests.forEach(async (test) => {
    const input = await readFile(test.input, "utf-8");
    const expectedOutput = await readFile(test.output, "utf-8");
    const { exitCode } = await execa("compiler", [input, `${test.output}.tmp`]);
    if (exitCode !== 0) {
      throw new Error(`compiler exited with code ${exitCode}`);
    }
    const output = await readFile(`${test.output}.tmp`, "utf-8");
    expect(output).toBe(expectedOutput);
  });
});
