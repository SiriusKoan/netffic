<template>
    <div class="inline-flex bg-opacity-40 bg-white items-center justify-center p-3">
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
