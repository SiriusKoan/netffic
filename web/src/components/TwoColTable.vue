<template>
    <table class="table table-zebra border-collapse">
        <thead>
            <tr class="h-auto">
                <th>{{ column1 }}</th>
                <th>{{ column2 }}</th>
            </tr>
        </thead>
        <tbody>
            <tr v-for="(val, key) in data" :key="key" class="h-auto">
                <td>{{ key }}</td>
                <td>{{ val }}</td>
            </tr>
        </tbody>
    </table>
</template>

<script>
export default {
    name: 'TwoColTable',
    props: {
        column1: String,
        column2: String,
        data: Array
    }
}
</script>
