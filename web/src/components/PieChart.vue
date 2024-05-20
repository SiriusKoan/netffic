<template>
    <div class="inline-flex bg-opacity-50 bg-white items-center justify-center p-3 rounded">
        <apexchart type="pie" :options="chartOptions" :series="chartSeries" width="500" />
    </div>
</template>

<script>
import VueApexCharts from "vue3-apexcharts";

export default {
    name: 'PieChart',
    components: {
        apexchart: VueApexCharts,
    },
    props: {
        chartOptions: Object,
        chartSeries: Array,
    },
};
</script>
